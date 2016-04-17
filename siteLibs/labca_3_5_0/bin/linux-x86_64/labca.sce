// Addinter for EZCA interface
// for hppa/sun-solaris/linux/dec
//--------------------------------
//Scilab functions 
labca_funs=[...
  'lcaGet';
  'lcaPut';
  'lcaPutNoWait';
  'lcaGetNelem';
  'lcaGetControlLimits';
  'lcaGetGraphicLimits';
  'lcaGetWarnLimits';
  'lcaGetAlarmLimits';
  'lcaGetStatus';
  'lcaGetPrecision';
  'lcaGetUnits';
  'lcaGetEnumStrings';
  'lcaGetRetryCount';
  'lcaSetRetryCount';
  'lcaGetTimeout';
  'lcaSetTimeout';
  'lcaDebugOn';
  'lcaDebugOff';
  'lcaSetSeverityWarnLevel';
  'lcaClear';
  'lcaSetMonitor';
  'lcaNewMonitorValue';
  'lcaNewMonitorWait';
  'lcaDelay';
  'lcaLastError';
  'lecdrGet';
  ];
labca_top=get_absolute_file_path('labca.sce')+'../../';
addinter(labca_top+'lib/linux-x86_64/libsezcaglue.so.0','labCA',labca_funs);
// a hack - addinter doesn't return a library handle
// (for unloading) -- obtain the last handle...
labca_hdl=link('show');
labca_hdl=labca_hdl($);
if msscanf(getversion(),"scilab-%i") < 5 then
  if grep(%helps(:,1),labca_top)== [] then
    %helps=[%helps;[labca_top+"html/help", "EPICS CA Interface"]];
  end
else
  if add_help_chapter('labCA',labca_top+'jar') then
	disp('Help files loaded')
  else
	disp('Loading help files FAILED')
  end
end

function lcaExit()
	del_help_chapter('labCA')
	ulink(labca_hdl)
endfunction
