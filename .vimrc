map <F9> :!make<CR>
map <F5> :!./pseudolisp<CR>


if has("gui_running")
	:!ctags -R .

	:e funkce.c
	:e structs.h
	:e testy.c
	:e execute.c
endif
