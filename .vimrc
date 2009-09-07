map <F9> :!make<CR>
map <F5> :!./pseudolisp<CR>


if has("gui_running")
	:!ctags -R .

	:e structs.h
	:e TODO
endif
