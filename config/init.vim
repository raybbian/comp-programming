call plug#begin()

Plug 'catppuccin/nvim', { 'as': 'catppuccin' }

Plug 'lervag/vimtex'
let g:tex_flavor='latex'
let g:vimtex_view_method='zathura'
let g:vimtex_quickfix_mode=0

Plug 'sirver/ultisnips'
let g:UltiSnipsExpandTrigger = '<tab>'
let g:UltiSnipsJumpForwardTrigger = '<tab>'
let g:UltiSnipsJumpBackwardTrigger = '<s-tab>'
let g:UltiSnipsSnippetDirectories = ['/home/rayb/OneDrive/Documents/config/']

Plug 'LunarWatcher/auto-pairs'
let g:AutoPairsCompleteOnlyOnSpace=1

call plug#end()

set relativenumber
set numberwidth=2
set clipboard+=unnamedplus
highlight clear Conceal

" Remember cursor position
augroup remember-cursor-position
	autocmd!
	autocmd BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g`\"" | endif
augroup END

" Start terminal in insert mode
augroup start-terminal-insert-mode
	autocmd!
	autocmd TermOpen * startinsert
augroup END

" Choose template on file creation
augroup choose-template-on-creation
	autocmd!
	autocmd BufNewFile /home/rayb/Documents/comp-programming/**.cpp execute "0r ~/Documents/comp-programming/templates/".input("Template name: ").".cpp | $ | -2"
augroup END

" Insert code snippet
function! InsertSnippet()
	execute "r ~/Documents/comp-programming/snippets/".input("Snippet name: ").".hpp"
endfunction
command! INS call InsertSnippet()

" Compile and Run
function! WriteCompileRun()
	w
	silent !cd %:p:h
	!g++ -std=c++20 -DLOCAL -Wall -Wextra -Wno-sign-conversion -O2 %:p
	terminal ./a.out
endfunction
command! WCR call WriteCompileRun()

" Run
function!  Run()
	silent !cd %:p:h
	terminal ./a.out
endfunction
command! R call Run()

