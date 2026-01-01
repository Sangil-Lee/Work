return {
  "szw/vim-maximizer",
  -- 키 단축키를 누를 때 플러그인을 로드하도록 설정 (Lazy Load)
  cmd = "MaximizerToggle",
  keys = {
    { "<leader>m", "<cmd>MaximizerToggle<CR>", desc = "Maximize/Minimize Window" },
  },
}
