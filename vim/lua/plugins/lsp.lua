return {
    "neovim/nvim-lspconfig",
    dependencies = {
        -- LSP 서버, 디버거, 린터 등을 관리하는 플러그인
        "williamboman/mason.nvim",
        "williamboman/mason-lspconfig.nvim",
        
        -- 자동 완성(Autocompletion) 기능과 연동하기 위한 플러그인
        "hrsh7th/cmp-nvim-lsp", 
    },
    config = function()
        local lspconfig = require("lspconfig")
        local mason = require("mason")
        local mason_lspconfig = require("mason-lspconfig")
        
        -- 1. Mason 설정 (LSP 서버 설치 관리자)
        mason.setup({
            ui = {
                icons = {
                    package_installed = "✓",
                    package_pending = "➜",
                    package_uninstalled = "✗"
                }
            }
        })

        -- 2. 설치할 언어 서버 목록 지정
        mason_lspconfig.setup({
            ensure_installed = {
                "pyright",       -- Python
                "clangd",        -- C/C++
                "jdtls",         -- Java
                "gopls",         -- Go
                "ts_ls",         -- JavaScript/TypeScript (구 tsserver)
                "lua_ls",        -- Lua (Neovim 설정용)
            },
            -- 설치된 서버들을 자동으로 설정(setup)하는 핸들러
            handlers = {
                function(server_name)
                    -- 자동 완성 기능(Capabilities) 연결
                    local capabilities = require("cmp_nvim_lsp").default_capabilities()
                    
                    -- 기본 설정 적용
                    lspconfig[server_name].setup({
                        capabilities = capabilities,
                    })
                end,

                -- 예외: Lua (Neovim 설정 시 전역 변수 'vim' 인식을 위해 추가 설정)
                ["lua_ls"] = function()
                    lspconfig.lua_ls.setup({
                        settings = {
                            Lua = {
                                diagnostics = { globals = { "vim" } },
                            },
                        },
                    })
                end,
                
                -- 예외: Java (jdtls는 복잡한 설정이 필요할 수 있으므로 기본만 연결)
                -- 심화 사용 시 'mfussenegger/nvim-jdtls' 플러그인 사용 권장
                ["jdtls"] = function()
                    lspconfig.jdtls.setup({
                        -- Java 관련 추가 설정이 필요하면 이곳에 작성
                    })
                end,
            }
        })

        -- 3. LSP 단축키 설정 (LspAttach 이벤트 사용)
        -- LSP가 연결된 버퍼에서만 단축키가 작동하도록 설정
        vim.api.nvim_create_autocmd("LspAttach", {
            group = vim.api.nvim_create_augroup("UserLspConfig", {}),
            callback = function(ev)
                local opts = { buffer = ev.buf }
                
                -- 주요 단축키
                vim.keymap.set("n", "gD", vim.lsp.buf.declaration, opts)        -- 선언부로 이동
                vim.keymap.set("n", "gd", vim.lsp.buf.definition, opts)         -- 정의부로 이동
                vim.keymap.set("n", "K", vim.lsp.buf.hover, opts)               -- 호버(정보 보기)
                vim.keymap.set("n", "gi", vim.lsp.buf.implementation, opts)     -- 구현부로 이동
                vim.keymap.set("n", "<C-k>", vim.lsp.buf.signature_help, opts)  -- 함수 시그니처 도움말
                vim.keymap.set("n", "<leader>rn", vim.lsp.buf.rename, opts)     -- 변수명 변경 (Rename)
                vim.keymap.set("n", "<leader>ca", vim.lsp.buf.code_action, opts)-- 코드 액션 (Code Action)
                vim.keymap.set("n", "gr", vim.lsp.buf.references, opts)         -- 참조 목록 보기
                
                -- 진단(Diagnostics) 이동
                vim.keymap.set("n", "[d", vim.diagnostic.goto_prev, opts)
                vim.keymap.set("n", "]d", vim.diagnostic.goto_next, opts)
            end,
        })
    end
}
