return {
  "mrjones2014/smart-splits.nvim",
  config = function()
    -- 추천 키매핑
    local sm = require("smart-splits")
    
    -- 크기 조절 (Alt + hjkl)
    vim.keymap.set("n", "<A-h>", sm.resize_left)
    vim.keymap.set("n", "<A-j>", sm.resize_down)
    vim.keymap.set("n", "<A-k>", sm.resize_up)
    vim.keymap.set("n", "<A-l>", sm.resize_right)

    -- 이동 (Ctrl + hjkl)
    vim.keymap.set("n", "<C-h>", sm.move_cursor_left)
    vim.keymap.set("n", "<C-j>", sm.move_cursor_down)
    vim.keymap.set("n", "<C-k>", sm.move_cursor_up)
    vim.keymap.set("n", "<C-l>", sm.move_cursor_right)
    
    -- Swapping (버퍼 위치 바꾸기 - Leader + hjkl)
    vim.keymap.set("n", "<leader><leader>h", sm.swap_buf_left)
    vim.keymap.set("n", "<leader><leader>j", sm.swap_buf_down)
    vim.keymap.set("n", "<leader><leader>k", sm.swap_buf_up)
    vim.keymap.set("n", "<leader><leader>l", sm.swap_buf_right)
  end,
}
