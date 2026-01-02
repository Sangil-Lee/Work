return {
    "hrsh7th/nvim-cmp",
    dependencies = {
        "hrsh7th/cmp-nvim-lsp",     -- LSP 소스
        "hrsh7th/cmp-buffer",       -- 현재 버퍼의 단어 추천
        "hrsh7th/cmp-path",         -- 파일 경로 추천
        "hrsh7th/cmp-cmdline",      -- 명령어 모드 추천
        "L3MON4D3/LuaSnip",         -- 스니펫 엔진
        "saadparwaiz1/cmp_luasnip", -- 스니펫 소스
        "rafamadriz/friendly-snippets", -- 미리 정의된 다양한 언어의 스니펫 모음
    },
    config = function()
        local cmp = require("cmp")
        local luasnip = require("luasnip")

        -- VSCode 스타일의 스니펫 로드
        require("luasnip.loaders.from_vscode").lazy_load()

        cmp.setup({
            snippet = {
                expand = function(args)
                    luasnip.lsp_expand(args.body)
                end,
            },
            window = {
                completion = cmp.config.window.bordered(), -- 테두리 있는 창
                documentation = cmp.config.window.bordered(),
            },
            mapping = cmp.mapping.preset.insert({
                ["<C-b>"] = cmp.mapping.scroll_docs(-4),
                ["<C-f>"] = cmp.mapping.scroll_docs(4),
                ["<C-Space>"] = cmp.mapping.complete(), -- 강제로 추천 창 띄우기
                ["<C-e>"] = cmp.mapping.abort(),        -- 추천 창 닫기
                ["<CR>"] = cmp.mapping.confirm({ select = true }), -- 엔터로 선택
                
                -- 탭 키로 이동 (선택 항목이 있으면 선택, 없으면 스니펫 이동)
                ["<Tab>"] = cmp.mapping(function(fallback)
                    if cmp.visible() then
                        cmp.select_next_item()
                    elseif luasnip.expand_or_jumpable() then
                        luasnip.expand_or_jump()
                    else
                        fallback()
                    end
                end, { "i", "s" }),

                ["<S-Tab>"] = cmp.mapping(function(fallback)
                    if cmp.visible() then
                        cmp.select_prev_item()
                    elseif luasnip.jumpable(-1) then
                        luasnip.jump(-1)
                    else
                        fallback()
                    end
                end, { "i", "s" }),
            }),
            sources = cmp.config.sources({
                { name = "nvim_lsp" }, -- LSP 추천 (함수, 변수 등)
                { name = "luasnip" },  -- 스니펫 추천
            }, {
                { name = "buffer" },   -- 현재 파일 내 단어 추천
                { name = "path" },     -- 파일 경로 추천
            }),
        })
    end,
}
