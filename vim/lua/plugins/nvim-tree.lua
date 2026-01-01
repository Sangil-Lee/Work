return {
  "nvim-tree/nvim-tree.lua",
  dependencies = {
    "nvim-tree/nvim-web-devicons", -- 아이콘 필수 플러그인
  },
  config = function()
    -- 1. Netrw 비활성화
    vim.g.loaded_netrw = 1
    vim.g.loaded_netrwPlugin = 1

    -- 2. 아이콘 색상 자동 적용 (Git 상태 등에 따라 색상 변경)
    -- 이 기능이 켜져 있어야 파일명이 빨간색/초록색으로 변합니다.
    vim.opt.termguicolors = true

    require("nvim-tree").setup({
      -- === UI 및 창 설정 ===
      view = {
        width = 30,
        side = "left",
        -- 라인 번호 제거 (깔끔함을 위해 false 추천, 필요하면 true)
        number = false,
        relativenumber = false,
        signcolumn = "yes", -- 아이콘이 잘리지 않도록 공간 확보
      },

      -- === 렌더링 (가장 중요한 부분) ===
      renderer = {
        root_folder_label = ":t", -- 루트 폴더 경로를 다 보여주지 않고 폴더명만 표시
        highlight_git = true,     -- Git 상태에 따라 파일명 색상 변경
        highlight_opened_files = "name", -- 현재 열린 파일을 강조 표시
        
        -- 들여쓰기 가이드라인 (계층 구조 선)
        indent_markers = {
          enable = true,
          inline_arrows = true,
          icons = {
            corner = "└",
            edge = "│",
            item = "│",
            bottom = "─",
            none = " ",
          },
        },
        
        -- 아이콘 커스터마이징
        icons = {
          show = {
            file = true,
            folder = true,
            folder_arrow = true,
            git = true,
          },
          glyphs = {
            default = "󰈚",
            symlink = "",
            folder = {
              arrow_open = "",
              arrow_closed = "",
              default = "",
              open = "",
              empty = "",
              empty_open = "",
              symlink = "",
              symlink_open = "",
            },
            git = {
              unstaged = "✗",
              staged = "✓",
              unmerged = "",
              renamed = "➜",
              untracked = "★",
              deleted = "",
              ignored = "◌",
            },
          },
        },
      },

      -- === 진단 (에러/경고 표시) ===
      diagnostics = {
        enable = true,
        show_on_dirs = true, -- 에러가 있는 파일이 든 폴더에도 아이콘 표시
        icons = {
          hint = "",
          info = "",
          warning = "",
          error = "",
        },
      },

      -- === 필터 (지저분한 파일 숨기기) ===
      filters = {
        dotfiles = false, -- .gitignore 파일 등을 보려면 false
        custom = { "^.git$", ".DS_Store" }, -- .git 폴더와 맥OS 쓰레기 파일 숨김
      },
      
      -- === 동작 설정 ===
      actions = {
        open_file = {
          quit_on_open = false, -- 파일 열어도 트리 유지
          resize_window = true,
        },
      },
    })

    -- 단축키 설정
    vim.keymap.set("n", "<leader>e", "<cmd>NvimTreeToggle<CR>", { desc = "Toggle Explorer" })
  end,
}
