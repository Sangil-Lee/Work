return {
	-- 1. Avante.nvim (채팅, 리팩토링, Diff 뷰어 용도)
	"yetone/avante.nvim",
	event = "VeryLazy",
	lazy = false,
	version = false, -- 최신 기능을 위해 false로 설정 (업데이트가 잦음)
	opts = {
		-- 1. 사용할 제공자를 지정
		provider = "gemini",

		-- 2. [수정됨] 상세 설정을 'providers' 테이블 안으로 이동
		providers = {
			gemini = {
				-- model = "gemini-1.5-flash-001", -- 또는 "gemini-1.5-pro"
				model = "gemini-2.5-flash", -- 또는 "gemini-1.5-pro"
				temperature = 0,
				max_tokens = 4096,
				-- 필요한 경우 API 키 환경변수 이름을 명시할 수 있습니다 (기본값: GEMINI_API_KEY)
			},
		},

		-- (선택 사항) 동작 방식 설정
		behaviour = {
			auto_suggestions = false, -- 입력 중 자동 제안 끄기 (취향에 따라 true)
			auto_set_highlight_group = true,
			auto_set_keymaps = true,
			auto_apply_diff_after_generation = false,
			support_paste_from_clipboard = false,
		},
	},

	-- 빌드 설정 (윈도우라면 'make' 대신 'powershell -ExecutionPolicy Bypass -File Build.ps1' 등을 써야 할 수 있음)
	build = "make",

	dependencies = {
		"stevearc/dressing.nvim",
		"nvim-lua/plenary.nvim",
		"MunifTanjim/nui.nvim",
		--- The below dependencies are optional,
		"hrsh7th/nvim-cmp", -- autocompletion for avante commands and mentions
		"nvim-tree/nvim-web-devicons", -- or echasnovski/mini.icons
		"zbirenbaum/copilot.lua", -- for providers='copilot'
	},
}
