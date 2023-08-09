call plug#begin()

Plug 'catppuccin/nvim', { 'as': 'catppuccin' }
Plug 'jiangmiao/auto-pairs'

call plug#end()

set relativenumber
set numberwidth=2

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
	autocmd BufNewFile /home/rayb/OneDrive/Documents/CompGramming/**.cpp execute "0r ~/Documents/comp-programming/templates/".input("Template name: ").".cpp | $ | -2"
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

