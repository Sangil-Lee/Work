return {
	"akinsho/bufferline.nvim",
	dependencies = { "nvim-tree/nvim-web-devicons" }, -- 파일 아이콘 지원
	version = "*",
	event = "VeryLazy", -- Neovim 시작 후 UI 로딩 시점에 로드
	keys = {
		{ "<leader>bp", "<Cmd>BufferLineTogglePin<CR>", desc = "Toggle Pin" },
		{ "<leader>bP", "<Cmd>BufferLineGroupClose ungrouped<CR>", desc = "Delete Non-Pinned Buffers" },
		{ "<leader>bo", "<Cmd>BufferLineCloseOthers<CR>", desc = "Delete Other Buffers" },
		{ "<leader>br", "<Cmd>BufferLineCloseRight<CR>", desc = "Delete Buffers to the Right" },
		{ "<leader>bl", "<Cmd>BufferLineCloseLeft<CR>", desc = "Delete Buffers to the Left" },
		{ "<S-h>", "<cmd>BufferLineCyclePrev<cr>", desc = "Prev Buffer" },
		{ "<S-l>", "<cmd>BufferLineCycleNext<cr>", desc = "Next Buffer" },
	},
	config = function()
		require("bufferline").setup({
			options = {
				-- 'ordinal': 버퍼 ID가 아니라 눈에 보이는 순서대로(1, 2, 3...) 번호를 매김
				numbers = "ordinal",

				-- 탭 닫기 버튼 끄기 (공간 절약)
				close_command = "bdelete! %d",
				right_mouse_command = "bdelete! %d",
				left_mouse_command = "buffer %d",
				middle_mouse_command = nil,

				-- 선택된 탭을 눈에 띄게 표시 (밑줄 등 스타일)
				indicator = {
					icon = "▎", -- this should be omitted if indicator style is not 'icon'
					style = "icon",
				},

				-- 긴 파일 경로 대신 파일명만 깔끔하게 표시
				show_buffer_close_icons = false,
				show_close_icon = false,
				separator_style = "thin", -- or "slant", "thick"
			},
		})

		-- [핵심] ,1 ~ ,9 단축키 매핑 자동 생성
		-- 눈에 보이는 순서(ordinal)대로 이동합니다.
		for i = 1, 9 do
			vim.keymap.set("n", "<leader>" .. i, function()
				require("bufferline").go_to_buffer(i, true)
			end, { desc = "Go to buffer " .. i })
		end
	end,
}
