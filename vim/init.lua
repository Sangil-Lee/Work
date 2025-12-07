-- ==========================================================================
-- 1. 기본 옵션 설정 (Basic Options)
-- ==========================================================================
vim.g.mapleader = " " -- Leader 키를 스페이스바로 설정
vim.opt.number = true -- 줄 번호 표시
vim.opt.relativenumber = true -- 상대 줄 번호 표시 (이동 편의)
vim.opt.mouse = "a" -- 마우스 사용 허용
vim.opt.clipboard = "unnamedplus" -- 시스템 클립보드 공유
vim.opt.tabstop = 4 -- 탭 크기
vim.opt.shiftwidth = 4 -- 들여쓰기 크기
vim.opt.expandtab = true -- 탭을 공백으로 변환
--vim.opt.ignorecase = true -- 검색 시 대소문자 무시
vim.opt.ignorecase = false -- 검색 시 대소문자 무시
vim.opt.smartcase = true -- 대문자 포함 시 대소문자 구별
vim.opt.termguicolors = true -- True Color 지원
-- 2. 기본 옵션 설정
vim.opt.softtabstop = 4
vim.opt.smartindent = true
vim.opt.wrap = false
vim.opt.swapfile = false
vim.opt.backup = false

-- 1. 일반 줄 번호 (LineNr): 노란색으로 변경
--vim.api.nvim_set_hl(0, "LineNr", { fg = "Yellow" })
vim.api.nvim_set_hl(0, "LineNr", { fg = "#F9E79F" })
-- 2. 현재 커서가 있는 줄 번호 (CursorLineNr): 노란색 + 굵게(Bold)
--    (보통 현재 줄은 더 강조하고 싶어하므로 Bold를 추가했습니다)
--vim.api.nvim_set_hl(0, "CursorLineNr", { fg = "Green", bold = true })
vim.api.nvim_set_hl(0, "CursorLineNr", { fg = "#F9E79F", bold = true })

-- 참고: 만약 상대 줄 번호(relativenumber)를 사용 중이라면 
-- LineNr은 주변 번호, CursorLineNr은 현재 위치의 절대 번호 색상이 됩니다.


vim.api.nvim_create_user_command('Table', "'<,'>!column -t", { range = true })
vim.api.nvim_create_user_command('CSV', "'<,'>!column -t -s,", { range = true })
vim.api.nvim_create_user_command('CSVO', "'<,'>!column -t -s ';' -o ';'", { range = true })

vim.api.nvim_create_user_command('AddBack', function(opts)
  -- 1. 사용자 입력값(Argument) 가져오기
  local suffix = opts.args

  -- 2. 입력값이 없으면(빈 문자열이면) 기본값 ':' 사용
  if suffix == "" or suffix == nil then
    suffix = ":"
  end

  -- 3. 명령어 조립
  -- 구분자를 '/' 대신 '#'를 사용했습니다. (입력값에 슬래시가 들어올 경우를 대비)
  -- \%V: 선택 영역 내부, \>: 단어의 끝
  local cmd = string.format("'<,'>s#\\%%V\\>#%s#g", suffix)

  -- 4. 명령어 실행
  vim.cmd(cmd)

end, {
  range = true, -- Visual 모드 범위 지원
  nargs = '?'   -- 인자를 0개 또는 1개 받음 ('?' 설정이 핵심)
})

vim.api.nvim_create_user_command('AddFront', function(opts)
  -- 1. 사용자 입력값(Argument) 가져오기
  local suffix = opts.args

  -- 2. 입력값이 없으면(빈 문자열이면) 기본값 ':' 사용
  if suffix == "" or suffix == nil then
    suffix = ":"
  end

  -- 3. 명령어 조립
  -- 구분자를 '/' 대신 '#'를 사용했습니다. (입력값에 슬래시가 들어올 경우를 대비)
  -- \%V: 선택 영역 내부, \>: 단어의 끝
  local cmd = string.format("'<,'>s#\\%%V\\<#%s#g", suffix)

  -- 4. 명령어 실행
  vim.cmd(cmd)

end, {
  range = true, -- Visual 모드 범위 지원
  nargs = '?'   -- 인자를 0개 또는 1개 받음 ('?' 설정이 핵심)
})

vim.api.nvim_create_user_command('Replace', function(opts)
  -- 1. 인자 가져오기 (fargs는 자동으로 공백 기준 분리된 테이블입니다)
  local target = opts.fargs[1]       -- 찾을 문자 (예: :)
  local replacement = opts.fargs[2]  -- 바꿀 문자 (예: ;)

  -- 2. 예외 처리: 두 번째 인자가 없으면 빈 문자열("")로 처리 (삭제 기능)
  if replacement == nil then
    replacement = ""
  end

  -- 3. 명령어 조립
  -- '<,'> : 선택된 범위
  -- s#...#...#g : 치환 명령 (#을 구분자로 사용)
  -- \%V : 비주얼 블록(Visual Block) 영역 안에서만 적용
  -- \V : "Very Nomagic" 모드 (입력한 문자를 정규식이 아닌 단순 문자로 취급)
  --      -> 이렇게 하면 점(.)이나 별(*)을 입력해도 정규식이 아닌 글자 그대로 찾습니다.
  
  local cmd = string.format("'<,'>s#\\%%V\\V%s#%s#g", target, replacement)

  -- 4. 실행
  -- pcall을 사용하여 찾는 문자가 없을 때 에러 메시지가 뜨는 것을 방지할 수도 있습니다.
  local success, msg = pcall(vim.cmd, cmd)
  if not success then
    print("Pattern not found: " .. target)
  end

end, {
  range = true, -- Visual 모드 범위 지원
  nargs = '+',  -- 인자가 1개 이상 필수 (최소 찾을 문자는 있어야 함)
})

vim.api.nvim_create_user_command('Remove', function(opts)
  local target = opts.args

  -- 1. 인자가 없을 때 (:Remove) -> 공백(White Space) 제거
  if target == "" or target == nil then
    -- \s+ : 스페이스나 탭이 1개 이상 연속된 것
    -- \%V : 비주얼 영역 내부
    vim.cmd("'<,'>s/\\%V\\s\\+//g")
    print("Removed all white spaces.")
  
  -- 2. 인자가 있을 때 (:Remove x) -> 해당 문자(x) 제거
  else
    -- \V : Very Nomagic (특수문자를 있는 그대로 취급)
    -- 교체할 문자열 부분을 비워둠(삭제)
    local cmd = string.format("'<,'>s#\\%%V\\V%s##g", target)
    vim.cmd(cmd)
    print("Removed pattern: " .. target)
  end

end, {
  range = true, -- Visual 모드 범위 지원
  nargs = '?'   -- 인자는 있어도 되고 없어도 됨
})

-- <Leader> + n 을 누르면 상대 줄 번호를 켰다 껐다 함
vim.keymap.set("n", "<leader>n", function()
  -- 현재 relativenumber가 켜져 있다면 끄고, 꺼져 있다면 킴
  if vim.opt.relativenumber:get() then
    vim.opt.relativenumber = false
    print("Relative Number: OFF")
  else
    vim.opt.relativenumber = true
    vim.opt.number = true -- 절대 번호는 항상 유지하는 것이 좋음
    print("Relative Number: ON")
  end
end, { desc = "Toggle Relative Number" })

-- ==========================================================================
-- 사용자 명령: Aligns (업그레이드 버전)
-- 기능: 
--   1. Visual Block(Ctrl+v) 선택 시: 해당 블록 내의 텍스트 기준으로 정렬
--   2. 그 외(일반/Visual Line): 라인의 첫 번째 단어(공백 제외) 기준으로 정렬
--   3. 결과는 클립보드에 복사됨 (원본 파일 수정 X)
-- ==========================================================================
vim.api.nvim_create_user_command('Aligns', function(opts)
  -- 1. 범위 설정
  local start_line = opts.line1 - 1
  local end_line = opts.line2
  local lines = vim.api.nvim_buf_get_lines(0, start_line, end_line, false)

  if #lines == 0 then return end

  -- 2. 직전 비주얼 모드 확인
  -- Ctrl-v는 Lua 문자열로 "\22"로 표현됩니다.
  local mode = vim.fn.visualmode()
  local is_block_mode = (mode == "\22")

  -- 3. 블록 모드일 경우 열(Column) 범위 계산
  local start_col, end_col
  if is_block_mode then
    -- '< 와 '> 마크는 비주얼 모드를 빠져나온 뒤 설정된 마지막 선택 영역의 위치입니다.
    local pos1 = vim.fn.getpos("'<")
    local pos2 = vim.fn.getpos("'>")
    
    start_col = pos1[3] -- 열 번호 (Byte Index)
    end_col = pos2[3]

    -- 역방향으로 드래그했을 경우를 대비해 순서 정렬
    if start_col > end_col then
      start_col, end_col = end_col, start_col
    end
  end

  -- 4. 정렬 로직 수행
  table.sort(lines, function(a, b)
    local a_key, b_key

    if is_block_mode then
      -- [블록 모드] 지정된 열 범위의 문자열을 잘라내어 키로 사용
      -- string.sub는 Byte 단위이므로 한글 중간이 잘리지 않게 주의 필요
      a_key = string.sub(a, start_col, end_col) or ""
      b_key = string.sub(b, start_col, end_col) or ""
      
      -- 잘라낸 블록 내에서도 앞뒤 공백은 무시하고 정렬하고 싶다면 아래 주석 해제
      -- a_key = a_key:match("^%s*(.*)") or ""
      -- b_key = b_key:match("^%s*(.*)") or ""
    else
      -- [기본 모드] 라인의 첫 단어(공백 제외) 추출
      a_key = a:match("^%s*(.*)") or ""
      b_key = b:match("^%s*(.*)") or ""
    end

    -- 대소문자 구분 없이 비교
    return a_key:lower() < b_key:lower()
  end)

  -- 5. 결과 처리 (클립보드 복사)
  local result = table.concat(lines, "\n")
  vim.fn.setreg('+', result) -- 시스템 클립보드
  vim.fn.setreg('"', result) -- 기본 레지스터

  local mode_msg = is_block_mode and "Vertical Block 기준" or "첫 단어 기준"
  print("✨ " .. #lines .. "개의 줄을 [" .. mode_msg .. "]으로 정렬하여 복사했습니다!")

end, { 
  range = "%", 
  desc = "선택 영역(Block/Line)에 따라 정렬하여 복사" 
})

-- ==========================================================================
-- 사용자 명령: DeleteLines
-- 기능: 선택 범위 내에서 특정 단어가 포함된 줄을 일괄 삭제
-- 사용법: :DeleteLines <검색어>
-- ==========================================================================
vim.api.nvim_create_user_command('DeleteLines', function(opts)
  local pattern = opts.args
  if pattern == "" then
    print("삭제할 검색어를 입력해주세요. (예: :DeleteLines error)")
    return
  end

  -- 1. 범위 설정 (1-based -> 0-based)
  local start_line = opts.line1 - 1
  local end_line = opts.line2

  -- 2. 범위 내의 모든 줄 가져오기
  local lines = vim.api.nvim_buf_get_lines(0, start_line, end_line, false)
  local kept_lines = {}
  local delete_count = 0

  -- 3. 필터링 로직
  for _, line in ipairs(lines) do
    -- plain=true: 정규식이 아닌 단순 문자열 매칭
    if string.find(line, pattern, 1, true) then
      -- 검색어가 포함된 줄이면 삭제 카운트 증가 (저장하지 않음)
      delete_count = delete_count + 1
    else
      -- 검색어가 없는 줄만 보존
      table.insert(kept_lines, line)
    end
  end

  -- 4. 결과 적용
  if delete_count > 0 then
    -- [핵심] 해당 범위의 내용을 남은 줄들(kept_lines)로 교체합니다.
    -- 이렇게 하면 삭제된 효과가 나며, Undo(u) 한 번으로 복구가 가능합니다.
    vim.api.nvim_buf_set_lines(0, start_line, end_line, false, kept_lines)
    print("🗑️ '" .. pattern .. "'이(가) 포함된 " .. delete_count .. "줄을 삭제했습니다.")
  else
    print("⚠️ 선택 범위 내에 '" .. pattern .. "'이(가) 포함된 줄이 없습니다.")
  end
end, { 
  nargs = 1, -- 인자 필수
  range = "%", -- 범위 지정 가능 (기본값: 전체 파일)
  desc = "범위 내에서 특정 단어가 포함된 줄 삭제" 
})


-- ==========================================================================
-- 사용자 명령: 범위(Range) 내에서 특정 단어가 포함된 라인 일괄 복사
-- 사용법: 
--   1. 전체 파일: :CopyLines <검색어>
--   2. 비주얼 모드: 드래그 후 :CopyLines <검색어>
--   3. 줄 번호 지정: :10,20CopyLines <검색어>
-- ==========================================================================
vim.api.nvim_create_user_command('CopyLines', function(opts)
  local pattern = opts.args
  if pattern == "" then
    print("검색어를 입력해주세요. (예: :CopyLines error)")
    return
  end

  -- [변경점 1] 범위(Range) 계산
  -- opts.line1은 1-based 시작 줄, opts.line2는 1-based 끝 줄입니다.
  -- nvim_buf_get_lines API는 0-based 인덱스를 사용하므로 변환이 필요합니다.
  local start_line = opts.line1 - 1
  local end_line = opts.line2

  -- [변경점 2] 지정된 범위의 줄만 가져오기
  local lines = vim.api.nvim_buf_get_lines(0, start_line, end_line, false)
  local matches = {}

  -- 루프를 돌며 패턴 찾기 (이전과 동일)
  for _, line in ipairs(lines) do
    if string.find(line, pattern, 1, true) then
      table.insert(matches, line)
    end
  end

  -- 결과 처리
  if #matches > 0 then
    local result = table.concat(matches, "\n")
    vim.fn.setreg('+', result) -- 시스템 클립보드
    vim.fn.setreg('"', result) -- 기본 레지스터
    
    print("✨ 선택 범위에서 '" .. pattern .. "' 포함 " .. #matches .. "줄 복사 완료!")
  else
    print("⚠️ 선택 범위 내에 '" .. pattern .. "'이(가) 없습니다.")
  end
end, { 
  nargs = 1, 
  range = "%", -- [핵심] 범위를 받도록 설정 (기본값: 파일 전체 %)
  desc = "지정된 범위에서 특정 단어가 포함된 줄을 복사" 
})

-- ==========================================================================
-- 사용자 명령: UniqueLines
-- 기능: 범위 내에서 중복된 줄을 제거하고 하나만 남김 (공백 무시 비교)
-- 사용법: :UniqueLines
-- ==========================================================================
vim.api.nvim_create_user_command('UniqueLines', function(opts)
  -- 1. 범위 설정
  local start_line = opts.line1 - 1
  local end_line = opts.line2

  -- 2. 라인 가져오기
  local lines = vim.api.nvim_buf_get_lines(0, start_line, end_line, false)
  
  local seen = {}      -- 이미 발견된 내용을 기록할 테이블
  local result = {}    -- 중복이 제거된 줄들을 담을 테이블
  local removed_count = 0

  for _, line in ipairs(lines) do
    -- [핵심] 비교를 위해 공백 제거 (CheckLines와 동일 로직)
    local normalized = line:gsub("%s+", "")

    -- 빈 줄인 경우 처리 (선택 사항: 여기서는 빈 줄도 중복되면 하나만 남김)
    -- 만약 빈 줄은 무조건 유지하고 싶다면 조건을 추가해야 함.

    if not seen[normalized] then
      -- 처음 본 내용이라면: 기록하고 결과에 추가
      seen[normalized] = true
      table.insert(result, line)
    else
      -- 이미 본 내용이라면: 삭제 카운트 증가 (결과에 추가 안 함)
      removed_count = removed_count + 1
    end
  end

  -- 3. 결과 적용 (중복이 있었을 때만 버퍼 수정)
  if removed_count > 0 then
    vim.api.nvim_buf_set_lines(0, start_line, end_line, false, result)
    print("✂️ 총 " .. removed_count .. "개의 중복 라인을 제거했습니다.")
  else
    print("✅ 중복된 라인이 없습니다.")
  end
end, { 
  range = "%", 
  desc = "범위 내 중복 라인 제거 (First one wins)" 
})

-- ==========================================================================
-- 사용자 명령: CheckLines & ClearChecks
-- 기능: 선택 범위 내에서 공백을 제외한 내용이 동일한 줄을 찾아 하이라이트
-- ==========================================================================

-- 1. 하이라이트 관리를 위한 네임스페이스 생성 (한 번만 실행됨)
local ns_id = vim.api.nvim_create_namespace('CheckLines')

vim.api.nvim_create_user_command('CheckLines', function(opts)
  -- 2. 기존에 표시된 CheckLines 하이라이트 제거 (초기화)
  vim.api.nvim_buf_clear_namespace(0, ns_id, 0, -1)

  local start_line = opts.line1 - 1
  local end_line = opts.line2
  
  -- 범위 내의 줄 가져오기
  local lines = vim.api.nvim_buf_get_lines(0, start_line, end_line, false)
  
  -- 3. 중복 검사를 위한 테이블 (Key: 정규화된 문자열, Value: 줄 번호 리스트)
  local map = {}

  for i, line in ipairs(lines) do
    -- [핵심] 모든 공백(스페이스, 탭) 제거하여 비교 기준 만들기
    local normalized = line:gsub("%s+", "")

    -- 빈 줄은 무시 (필요하다면 이 조건을 제거해도 됨)
    if normalized ~= "" then
      if not map[normalized] then
        map[normalized] = {}
      end
      -- 현재 줄의 절대 번호 저장 (0-based)
      table.insert(map[normalized], start_line + i - 1)
    end
  end

  -- 4. 중복된 줄 하이라이트 적용
  local dup_count = 0
  for _, line_nums in pairs(map) do
    -- 같은 내용이 2개 이상인 경우만 처리
    if #line_nums > 1 then
      dup_count = dup_count + 1
      for _, lnum in ipairs(line_nums) do
        -- 하이라이트 적용 (색상 그룹: 'Search' 사용 - 보통 노란색 배경)
        -- 다른 색을 원하면 'Error', 'WarningMsg', 'Visual' 등으로 변경 가능
        vim.api.nvim_buf_add_highlight(0, ns_id, 'Search', lnum, 0, -1)
      end
    end
  end

  if dup_count > 0 then
    print("🔍 " .. dup_count .. "개의 중복 그룹을 발견하여 표시했습니다.")
  else
    print("✅ 중복된 라인이 없습니다 (공백 제외).")
  end
end, { 
  range = "%", 
  desc = "공백 제외 동일한 라인 하이라이트" 
})

-- ==========================================================================
-- 보너스 명령: 하이라이트 끄기
-- ==========================================================================
vim.api.nvim_create_user_command('ClearChecks', function()
  vim.api.nvim_buf_clear_namespace(0, ns_id, 0, -1)
  print("🧹 하이라이트를 제거했습니다.")
end, { desc = "CheckLines 하이라이트 제거" })

vim.keymap.set("n", "<C-a>", "ggVG", { desc = "Select All" })
-- (선택 사항) 입력 모드(Insert Mode)에서도 작동하게 하려면 추가
vim.keymap.set("i", "<C-a>", "<Esc>ggVG", { desc = "Select All in Insert Mode" })

-- lazy.nvim 설치 (Bootstrap)
local lazypath = vim.fn.stdpath("data") .. "/lazy/lazy.nvim"
if not vim.loop.fs_stat(lazypath) then
  vim.fn.system({
    "git",
    "clone",
    "--filter=blob:none",
    "https://github.com/folke/lazy.nvim.git",
    "--branch=stable",
    lazypath,
  })
end
vim.opt.rtp:prepend(lazypath)
require("lazy").setup({
  
  {
	"nvim-treesitter/nvim-treesitter",
	build = ":TSUpdate",
	config = function()
	  require("nvim-treesitter.configs").setup({
	    -- 여기에 "lua"가 있는지 확인하고 없다면 추가!
	    ensure_installed = { "c","cpp", "java", "vim", "go","gomod","gosum", "rust", "lua" },

	    -- 자동 설치 활성화 (권장)
	    auto_install = true,

	    highlight = { enable = true },
	    indent = { enable = true },
	  })
	end,
  },

    
  -- Mason.nvim: LSP, 포맷터, 린터 등을 쉽게 설치/관리
  {
    "williamboman/mason.nvim",
    config = function()
      require("mason").setup()
    end,
  },

  -- Mason-lspconfig.nvim: Mason과 nvim-lspconfig를 연결
  {
    "williamboman/mason-lspconfig.nvim",
    -- mason.nvim이 먼저 로드된 후에 실행되도록 종속성 설정
    dependencies = { "williamboman/mason.nvim" },
  },

  {
    "junegunn/vim-easy-align",
  },

  -- Nvim-lspconfig: 언어 서버 설정의 핵심
  {
  "neovim/nvim-lspconfig",
  dependencies = {
    -- LSP 서버를 자동으로 설치하고 관리해주는 도구들
    "williamboman/mason.nvim",
    "williamboman/mason-lspconfig.nvim",

    -- (선택사항) nvim-cmp 자동완성 UI 플러그인과의 연동을 위함
    "hrsh7th/nvim-cmp",
    "hrsh7th/cmp-nvim-lsp",
  },
  config = function()
    -- ## 1. 공통 설정 정의 (DRY: Don't Repeat Yourself) ##
    -- 모든 LSP 서버에 공통으로 적용될 키맵과 설정을 on_attach 함수 안에 정의합니다.
    local on_attach = function(client, bufnr)
      -- LSP 기능 활성화 상태를 시각적으로 표시 (선택사항)
      vim.api.nvim_create_autocmd("CursorHold", {
        buffer = bufnr,
        callback = function()
          local opts = {
            focusable = false,
            close_events = { "BufLeave", "CursorMoved", "InsertEnter", "FocusLost" },
            border = "rounded",
            source = "always",
            prefix = " ",
            scope = "cursor",
          }
          vim.diagnostic.open_float(nil, opts)
        end,
      })
      
      -- 공통 키맵 설정
      local opts = { buffer = bufnr, remap = false }
      vim.keymap.set("n", "gd", vim.lsp.buf.definition, opts)
      vim.keymap.set("n", "K", vim.lsp.buf.hover, opts)
      vim.keymap.set("n", "gD", vim.lsp.buf.declaration, opts)
      vim.keymap.set("n", "gi", vim.lsp.buf.implementation, opts)
      vim.keymap.set("n", "gr", vim.lsp.buf.references, opts)
      vim.keymap.set("n", "<leader>ca", vim.lsp.buf.code_action, opts)
      vim.keymap.set("n", "<leader>rn", vim.lsp.buf.rename, opts)
      vim.keymap.set("n", "<leader>d", vim.diagnostic.open_float, opts)
      vim.keymap.set("n", "[d", vim.diagnostic.goto_prev, opts)
      vim.keymap.set("n", "]d", vim.diagnostic.goto_next, opts)
      vim.keymap.set("x", "ga", "<Plug>(EasyAlign)")
      vim.keymap.set("n", "ga", "<Plug>(EasyAlign)")
    end

    -- nvim-cmp 자동완성과 연동하기 위한 capabilities 설정
    local capabilities = require('cmp_nvim_lsp').default_capabilities()

    -- ## 2. 관리할 LSP 서버 목록 ##
    -- 여기에 필요한 언어 서버 이름을 추가하기만 하면 자동으로 설치 및 설정됩니다.
    local servers = {
      "lua_ls", -- Neovim 자체 설정을 위함
    }

    -- ## 3. 자동 설치 및 설정 루프 ##
    -- mason을 통해 서버 목록을 자동으로 설치하도록 설정
    require("mason-lspconfig").setup({
      ensure_installed = servers,
    })

    -- 서버 목록을 순회하며 각 서버에 공통 설정을 적용
    for _, server_name in ipairs(servers) do
      -- 모든 서버에 적용할 기본 설정
      local server_opts = {
        on_attach = on_attach,
        capabilities = capabilities,
      }

      -- lspconfig를 사용하여 최종 설정 적용
      -- require("lspconfig")[server_name].setup(server_opts)
    end
  end,
  },
  
})

