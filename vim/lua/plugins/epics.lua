return {
	-- [수정됨] 존재하는 유효한 저장소로 변경
	"nickez/epics.vim",
	ft = { "epics" },
	init = function()
		vim.filetype.add({
			extension = {
				db = "epics",
				template = "epics",
				substitutions = "epics",
				cmd = "epics",
			},
			filename = {
				["st.cmd"] = "epics",
			},
		})
	end,
}
