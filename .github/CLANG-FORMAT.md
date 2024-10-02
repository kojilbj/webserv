Installation of clang-format

for vscode

1. Install https://marketplace.visualstudio.com/items?itemName=xaver.clang-format as vscode extension.
2. To automatically format a file on save, add the following to your vscode settings.json file:

{
    "editor.formatOnSave": true
}

3. reopen vscode.
4. edit .clang-format file in the root of this repo if necessary.

for vim

1. add plugin 'rhysd/vim-clang-format' in your .vimrc.
2. go to ~/.vim/autoload/clang_format.vim.
3. change a line.
   let g:clang_format#auto_format = s:getg('clang_format#auto_format', 0)
   ||
   \/
   let g:clang_format#auto_format = s:getg('clang_format#auto_format', 1)
4. edit .clang-format file in the root of this repo if necessary.
