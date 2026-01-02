return {
    "stevearc/conform.nvim",
    event = { "BufReadPre", "BufNewFile" },
    config = function()
        require("conform").setup({
            formatters_by_ft = {
                -- 각 언어별 포맷터 지정
                lua = { "stylua" },
                python = { "isort", "black" }, -- import 정렬(isort) 후 포맷팅(black)
                javascript = { "prettier" },
                typescript = { "prettier" },
                go = { "gofumpt" },            -- gofmt의 엄격한 버전
                c = { "clang-format" },
                cpp = { "clang-format" },
                java = { "google-java-format" }, 
            },
            -- 저장 시 자동 포맷팅 옵션
            format_on_save = {
                lsp_fallback = true,   -- 포맷터가 없으면 LSP 기능 사용
                async = false,
                timeout_ms = 500,
            },
        })
    end,
}
