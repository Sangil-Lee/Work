return {
	"akinsho/toggleterm.nvim",
	version = "*",
	config = function()
		-- 1. ToggleTerm 기본 설정
		require("toggleterm").setup({
			size = 20,
			open_mapping = [[<c-\>]],
			hide_numbers = true,
			shade_terminals = true,
			start_in_insert = true,
			direction = "float",
			close_on_exit = true,
			shell = vim.o.shell,
		})

		-- 2. Claude Code 터미널 인스턴스 생성
		local Terminal = require("toggleterm.terminal").Terminal

		local claude_term = Terminal:new({
			cmd = "claude", -- 만약 실행 안 되면 절대경로(예: "/usr/bin/claude")로 변경
			dir = "git_dir", -- Git 프로젝트 루트에서 실행
			direction = "float",
			float_opts = {
				border = "curved",
				width = 120,
				height = 30,
			},
			-- 터미널 열릴 때 Insert 모드 진입 및 ESC 매핑
			on_open = function(term)
				vim.cmd("startinsert!")
				-- 터미널 안에서 ESC 누르면 닫기
				vim.keymap.set("t", "<Esc>", "<cmd>close<CR>", { buffer = term.bufnr, noremap = true, silent = true })
			end,
			-- 닫힐 때 파일 변경 체크 (Neovim 버퍼 갱신)
			on_close = function(term)
				vim.cmd("checktime")
			end,
		})

		-- 3. [핵심 수정] 토글 키매핑 (vim.keymap.set 사용)
		-- 함수 내부에서 claude_term 지역 변수를 직접 참조하므로 에러가 나지 않습니다.
		vim.keymap.set("n", "<leader>cc", function()
			claude_term:toggle()
		end, { noremap = true, silent = true, desc = "Toggle Claude Code CLI" })
	end,
}
