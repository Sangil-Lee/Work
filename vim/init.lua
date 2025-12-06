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

