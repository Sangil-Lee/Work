return {
	"olimorris/codecompanion.nvim",
	dependencies = {
		"nvim-lua/plenary.nvim",
		"nvim-treesitter/nvim-treesitter",
	},
	config = function()
		require("codecompanion").setup({
			strategies = {
				-- [변경] 어댑터 이름을 'gemini'가 아닌 'my_gemini'로 연결
				chat = { adapter = "gemini" },
				inline = { adapter = "gemini" },
				agent = { adapter = "gemini" },
			},
			adapters = {
				-- [변경] 여기서 'my_gemini'라는 이름으로 새로 정의
				gemini = function()
					return require("codecompanion.adapters").extend("gemini", {
						env = {
							api_key = os.getenv("GEMINI_API_KEY"),
						},
						schema = {
							model = {
								-- 확실한 모델명 지정
								default = "gemini-2.5-flash",
							},
						},
					})
				end,
			},
		})
	end,
}
