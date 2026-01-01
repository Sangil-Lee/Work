return {
  "catppuccin/nvim",
  name = "catppuccin",
  priority = 1000,
  config = function()
    require("catppuccin").setup({
      integrations = {
        nvimtree = true, -- Nvim-Tree 전용 색상 지원 활성화
      }
    })
    vim.cmd.colorscheme "catppuccin"
  end
}
