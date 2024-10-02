A. Installation of clang-format

    execute "brew install clang-format" or "apt install clang-format".

    for vscode:
    
    1. Install https://marketplace.visualstudio.com/items?itemName=xaver.clang-format as vscode extension.
    2. To automatically format a file on save, add the following to your vscode settings.json file:
    
    {
        "editor.formatOnSave": true
    }
    
    3. reopen vscode.
    4. edit .clang-format file in the root of this repo if necessary.
    
    for vim:
    
    1. add plugin 'rhysd/vim-clang-format' in your .vimrc.
    2. go to ~/.vim/autoload/clang_format.vim.
    3. change a line.
       let g:clang_format#auto_format = s:getg('clang_format#auto_format', 0)
       ||
       \/
       let g:clang_format#auto_format = s:getg('clang_format#auto_format', 1)
    4. edit .clang-format file in the root of this repo if necessary.

B. Installation of clang-tidy

    execute "brew install llvm -> ln -s "$(brew --prefix llvm)/bin/clang-tidy" "/usr/local/bin/clang-tidy" " or "apt install clang-tidy".
    
    for vscode:

    1. Install https://marketplace.visualstudio.com/items?itemName=CS128.cs128-clang-tidy as vscode extension.
    2. 2. To automatically format a file on save, add the following to your vscode settings.json file:
    
    {
        "clang-tidy.fixOnSave": true
    }
    3. reopen vscode.
    4. edit .clang-tidy file in the root of this repo if necessary.

    for vim:

    1. no such plugin.
    2. execute "clang-tidy <file>" on terminal.
    3. edit .clang-tidy file in the root of this repo if necessary.
