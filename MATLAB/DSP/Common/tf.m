function TFsys = tf(varargin)
%TF  Creation of transfer functions or conversion to transfer function.
%
%  Creation:
%    SYS = TF(NUM,DEN) creates a continuous-time transfer function SYS with 
%    numerator(s) NUM and denominator(s) DEN.  The output SYS is a TF object.  
%
%    SYS = TF(NUM,DEN,TS) creates a discrete-time transfer function with
%    sample time TS (set TS=-1 if the sample time is undetermined).
%
%    S = TF('s') specifies the transfer function H(s) = s (Laplace variable).
%    Z = TF('z',TS) specifies H(z) = z with sample time TS.
%    You can then specify transfer functions as rational expressions in 
%    S or Z, e.g.,
%       s = tf('s');  H = (s+1)/(s^2+3*s+1)
%
%    SYS = TF creates an empty TF object.
%    SYS = TF(M) specifies a static gain matrix M.
%
%    In all syntax above, the input list can be followed by pairs
%       'PropertyName1', PropertyValue1, ...
%    that set the various properties of TF models (see LTIPROPS for details).  
%    To make SYS inherit all its LTI properties from an existing LTI model
%    REFSYS, use the syntax SYS = TF(NUM,DEN,REFSYS).
%
%  Data format:
%    For SISO models, NUM and DEN are row vectors listing the numerator and
%    denominator coefficients in 
%     * descending powers of s or z by default
%     * ascending powers of q = z^-1 if the 'Variable' property is set to  
%       'z^-1' or 'q' (DSP convention).
%
%    For MIMO models with NY outputs and NU inputs, NUM and DEN are NY-by-NU
%    cell arrays of row vectors where NUM{i,j} and DEN{i,j} specify the 
%    transfer function from input j to output i.  For example,
%       H = TF( {-5 ; [1 -5 6]} , {[1 -1] ; [1 1 0]})
%    specifies the two-output, one-input transfer function
%       [     -5 /(s-1)      ]
%       [ (s^2-5s+6)/(s^2+s) ] 
%
%    By default, transfer functions are displayed as functions of 's' or 'z'.
%    Alternatively, you can set the variable name to 'p' (continuous time) 
%    and 'z^-1' or 'q' (discrete time) by modifying the 'Variable' property.
%
%  Arrays of transfer functions:
%    You can create arrays of transfer functions by using ND cell arrays for
%    NUM and DEN above.  For example, if NUM and DEN are cell arrays of size
%    [NY NU 3 4], then 
%       SYS = TF(NUM,DEN) 
%    creates the 3-by-4 array of transfer functions
%       SYS(:,:,k,m) = TF(NUM(:,:,k,m),DEN(:,:,k,m)),  k=1:3,  m=1:4.
%    Each of these transfer functions has NY outputs and NU inputs.
%
%    To pre-allocate an array of zero transfer functions with NY outputs
%    and NU inputs, use the syntax
%       SYS = TF(ZEROS([NY NU k1 k2...])) .
%
%  Conversion:
%    SYS = TF(SYS) converts an arbitrary LTI model SYS to the transfer 
%    function representation. The result is a TF object.  
%
%    SYS = TF(SYS,'inv') uses a fast algorithm for conversion from state
%    space to TF, but is typically less accurate for high-order systems.
%
%  See also LTIMODELS, FILT, SET, GET, TFDATA, SUBSREF, LTIPROPS, ZPK, SS.

% Note:
%     TFSYS = TF(SYS,method)  allows users to supply their own conversion 
%     algorithm METHOD.  For instance,
%          tf(sys,'myway')
%     executes
%          [num,den] = myway(sys.a,sys.b,sys.c,sys.d,sys.e)
%     to perform the conversion to TF.  User-specified functions 
%     should follow this syntax.

%     Author(s): A. Potvin, 3-1-94, P. Gahinet, 4-5-96
%     Copyright 1986-2002 The MathWorks, Inc. 
%     $Revision: 1.28 $  $Date: 2002/05/10 19:55:53 $

% Effect on other properties
% Keep all LTI parent fields

sys = varargin{1};

% Handle syntax TF(num,den,sys) with sys of class SS
if ~isa(sys,'ss'),   
  nlti = 0;
  for i=1:length(varargin),
     if isa(varargin{i},'lti'), 
        nlti = nlti + 1;   ilti = i;
     end
  end

  if nlti>1, 
     error('Cannot call TF with several LTI arguments.');
  else 
     % Replace sys by sys.lti and call constructor tf/tf.m
     varargin{ilti} = varargin{ilti}.lti;
     TFsys = tf(varargin{:});
     return
  end
end

% Error checking
ni = nargin;
if ni>2,
   error('Conversion from SS to TF: too many input arguments.');
elseif ni==2 & ~ischar(varargin{2}),
   error('Conversion from SS to TF: second argument must be a string.');
elseif ni==1,
   method = 'tzero';
else
   method = varargin{2};
end


% Check for simple cases
sizes = size(sys.d);
ny = sizes(1);
nu = sizes(2);
if any(sizes==0),
   % Empty system
   TFsys = tf(cell(sizes),cell(sizes),sys.lti);
   return
elseif ~any(strcmp(method,{'inv','tzero'})),
   % User-specified conversion method
   [num,den] = feval(method,sys.a,sys.b,sys.c,sys.d,sys.e);
   TFsys = tf(num,den,sys.lti);
   return
end


% Conversion starts.
num = cell(sizes);
den = cell(sizes);
for k=1:prod(sizes(3:end)),
   ak = sys.a{k};
   bk = sys.b{k};
   ck = sys.c{k};
   dk = sys.d(:,:,k);
   ek = sys.e{k};
   if all(isfinite(dk(:)))
      nx = size(ak,1);
      DenList = struct('states',cell(1,0),'den',cell(1,0));
      
      % Compute each entry of the transfer function
      for l=1:ny*nu,
         j = 1+floor((l-1)/ny);
         i = 1+rem(l-1,ny);
         
         % Eliminate structurally nonminimal dynamics in sys(i,j,k)
         [ar,br,cr,er,ixr] = smreal(ak,bk(:,j),ck(i,:),ek,(1:nx)');
         
         % Compute the denominator 
         % RE: Avoid recomputing the same denominator twice by
         %     storing result along with state vector portion IXR
         [den{i,j,k},DenList] = getden(ar,er,ixr,DenList);
         
         % Compute the numerator
         if strcmp(method,'tzero'),
            % REVISIT : update tzero (GETZER in tests/lti more accurate) + 
            %           make it handle descriptor [z,k] = tzero(a,b,c,d,e);
            %           TZERO should also make sure z come in conj. pairs if a & e real
            % Use GETZEROS to compute the numerator of TFsys(i,j)
            [z,g] = getzeros(ar,br,cr,dk(i,j),er);
            num{i,j,k} = g * poly(z);
            if g==0,
               den{i,j,k} = 1;
            end
         else
            % Use GETNUM to compute the numerator of TFsys(i,j)
            num{i,j,k} = getnum(ar,br,cr,dk(i,j),er,den{i,j,k});
         end
         
         % Check for overflow
         if ~all(isfinite(num{i,j,k})) | ~all(isfinite(den{i,j,k}))
            error('Overflow in conversion to transfer function (TF) form.')
         end
      end
   else
      num(:,:,k) = {NaN};
      den(:,:,k) = {1};
   end
end
   
lwarn = lastwarn;warn = warning('off');
TFsys = tf(num,den,sys.lti);
warning(warn);lastwarn(lwarn);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [den,DenList] = getden(ar,er,ixr,DenList)
%GETDEN   Derives DEN = det(s*Er-Ar) where Ar=A(IXR,IXR) by
%           * first looking up in the DENLIST table for a state 
%             combination that matches IXR
%           * computing the gen. eigenvalues of (Ar,Er) if IXR is 
%             a new state combination
%         DENLIST keeps track of previously computed denominators 
%         (along with the corresponding state vector portion IXR)

if isempty(ixr),
    den = 1;
    return
else
    ldl = length(DenList);
    for j=1:ldl,
        if isequal(ixr,DenList(j).states),
            den = DenList(j).den;
            return
        end
    end
end

% DEN is not tabulated yet (IXR is a new subset of the state vector)
if isempty(er)
    p = eig(ar);
else
    p = eig(ar,er);
end
den = poly(p);
DenList(1,ldl+1).states = ixr;
DenList(1,ldl+1).den = den;


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function num = getnum(a,b,c,d,e,den)
%GETNUM   Computes the numerator NUM such that NUM/DEN is the transfer
%         function of the SISO system with state-space data (A,B,C,D,E).

% Shift D away from zero
dshft = 0;
if abs(d) < sqrt(eps),
    dshft = pow2(nextpow2(1e-4*norm(b)*norm(c)/(1+norm(a,1))));
    if d<0,  dshft = -dshft;  end
end
dd = d + dshft;

% Compute N such that  N(s)/DEN(s) = DD + C * inv(sE-A) * B .
% To this end, determine the roots of N(s) as the poles of the 
% inverse system.
ainv = a - b * c / dd;
if isempty(e)
    z = eig(ainv);
else
    z = eig(ainv,e);
end
num = dd * poly(z);

% Undo shift of D to get true NUM
num = num - dshft * den;

% In strictly proper case, remove small parasitic entries in NUM
% by computing cb,cAb,...  ZCOEFF keeps track of the number of zero 
% leading coefficients in NUM.
if d==0,
   zcoeff = 1;
   Ab = b;
   while zcoeff<length(num)-1 & c*Ab==0,
      % RE: the test c*Ab=0 is free of rounding error for companion 
      % realizations due to the particular structure of A,b,c. It 
      % should not be replaced by |c*Ab| < eps * norm(c) * norm(Ab)
      % since |c*Ab| may be very small even for nonzero coefficients
      % of num
      zcoeff = zcoeff + 1;
      Ab = a * Ab;
   end
   num(:,1:zcoeff) = 0;
end

