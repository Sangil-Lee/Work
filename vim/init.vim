"=========================Basic Setting==================================================
set cindent " C언어 자동 들여쓰기
set smartindent
set autoindent " 자동 들여쓰기
"set hlsearch " 검색어 하이라이팅
set nowrap
set ts=4 "tag select
set sw=1 " 스크롤바 너비
set sts=4 "st select
set ruler " 현재 커서 위치 표시
set hls!
"set encoding=utf-8
"set fileencodings=utf-8,cp949
set nu " 줄번호
set scrolloff=2
set wildmode=longest,list
set autowrite " 다른 파일로 넘어갈 때 자동 저장
set autoread " 작업 중인 파일 외부에서 변경됬을 경우 자동으로 불러옴
set bs=eol,start,indent
set history=256
set laststatus=2 " 상태바 표시 항상
set paste " 붙여넣기 계단현상 없애기
set shiftwidth=4 " 자동 들여쓰기 너비 설정
set showmatch " 일치하는 괄호 하이라이팅
set smartcase " 검색시 대소문자 구별
set smarttab
set softtabstop=4
set tabstop=4
set incsearch
set clipboard=unnamedplus
set statusline=\ %<%l:%v\ [%P]%=%a\ %h%m%r\ %F\
			   " 마지막으로 수정된 곳에 커서를 위치함
			   au BufReadPost *
			   \ if line("'\"") > 0 && line("'\"") <= line("$") |
			   \ exe "norm g`\"" |
			   \ endif
			   " 파일 인코딩을 한국어로
			   if $LANG[0]=='k' && $LANG[1]=='o'
			   set fileencoding=korea
			   endif
			   " 구문 강조 사용
			   if has("syntax")
			   syntax on
			   endif
			   " 컬러 스킴 사용
colorscheme torte
filetype on
au BufNewFile,BufRead *.stt set filetype=cpp
syn on
"=========================Map=========================
map <F2> v]}zf
map <F3> zo
map <F4> 

map ,1 :b!1<CR>
map ,2 :b!2<CR>
map ,3 :b!3<CR>
map ,4 :b!4<CR>
map ,5 :b!5<CR>
map ,6 :b!6<CR>
map ,7 :b!7<CR>
map ,8 :b!8<CR>

"=========================ctags=========================
set tags=~/kernelsrc/linux-3.9.2/tags

"=========================cscope=========================
set csprg=/usr/bin/cscope	"cscope program
set csto=0					"cscope DB search list
set cst						"cscope DB tag DB search
set nocsverb				"verbose off

"cscope DB의 위치 설정
cs add ~/kernelsrc/linux-3.9.2/cscope.out ~/kernelsrc/linux-3.9.2
set csverb "verbose off

"Tag List 환경설정
filetype on "vim filetype on
nmap <F7> :TlistToggle<CR> "F7 key = Tag List Toggling
let Tlist_Ctags_Cmd = "/usr/bin/ctags"  "ctags 프로그램 위치
let Tlist_Inc_Winwidth = 0				"window width change off
let Tlist_Exit_OnlyWindow = 0			"tag/file 선택 완료 시 taglist window close =off
let Tlist_Auto_Open = 0					"vim 시작 시 window open = off
let Tlist_Use_Right_Window = 1			"vim 시작 시 window open = off

"Source Explorer 환경설정
nmap <F8> :SrcExplToggle<CR>	"F8 key = SrcExpl Toggling
nmap <C-H> <C-W>h				"왼쪽 창으로 이동
nmap <C-J> <C-W>j				"하단(preview) 창으로 이동
nmap <C-K> <C-W>k				"상단 창으로 이동
nmap <C-L> <C-W>l				"오른쪽 창으로 이동

let g:SrcExpl_winHeight = 8			"SrcExpl 윈도우 높이 지정
let g:SrcExpl_refreshTime = 100		"refreshing time = 100ms
let g:SrcExpl_jumpKey = "<ENTER>"	"해당 definition으로 jump
let g:SrcExpl_gobackKey = "<SPACE>" "back
let g:SrcExpl_isUpdateTags = 0		"tag file update = off

"NERD Tree
let NERDTreeWinPos = "left"			"NERD Tree위치 = 왼쪽
nmap <F9> :NERDTreeToggle<CR>		"F9 key = NERD Tree Toggling
