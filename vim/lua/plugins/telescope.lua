return {
  "nvim-telescope/telescope.nvim",
  tag = "0.1.5", -- 혹은 branch = '0.1.x'
  dependencies = {
    "nvim-lua/plenary.nvim",
    -- 검색 속도를 C언어 수준으로 높여주는 확장 기능 (Make, GCC 필요)
    { 
      "nvim-telescope/telescope-fzf-native.nvim", 
      build = "make" 
    },
  },
  config = function()
    local telescope = require("telescope")
    local actions = require("telescope.actions")

    telescope.setup({
      defaults = {
        path_display = { "truncate " }, -- 파일 경로가 너무 길면 줄임
        mappings = {
          i = {
            ["<C-k>"] = actions.move_selection_previous, -- 이전 항목 이동 (Ctrl+k)
            ["<C-j>"] = actions.move_selection_next,     -- 다음 항목 이동 (Ctrl+j)
            ["<C-q>"] = actions.send_selected_to_qflist + actions.open_qflist, -- 선택 항목 퀵픽스로 보내기
            
            -- [화면 분할 최적화 매핑]
            ["<C-v>"] = actions.select_vertical,   -- 검색 결과 세로 분할로 열기
            ["<C-x>"] = actions.select_horizontal, -- 검색 결과 가로 분할로 열기
          },
        },
      },
    })

    -- FZF 확장 기능 로드
    telescope.load_extension("fzf")

    -- === 추천 단축키 설정 ===
    local keymap = vim.keymap.set
    -- 1. 파일 찾기 (파일명 검색)
    keymap("n", "<leader>ff", "<cmd>Telescope find_files<cr>", { desc = "Fuzzy find files" })
    -- 2. 글자 찾기 (전체 프로젝트 내 텍스트 검색 - Ripgrep 필요)
    keymap("n", "<leader>fg", "<cmd>Telescope live_grep<cr>", { desc = "Find text in project" })
    -- 3. 현재 열려있는 버퍼 찾기
    keymap("n", "<leader>fb", "<cmd>Telescope buffers<cr>", { desc = "Find open buffers" })
    -- 4. 도움말 태그 찾기
    keymap("n", "<leader>fh", "<cmd>Telescope help_tags<cr>", { desc = "Find help tags" })
  end,
}
