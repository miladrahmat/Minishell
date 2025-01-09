# Minishell
<p align=center>
  <img src="https://github.com/miladrahmat/42-badges/blob/master/badges/minishelle.png"><img height="144" src="https://github.com/user-attachments/assets/5f720add-180f-40c3-b079-fdf89f200b6d"
</p>

<p align=center>
  <b>A small unix shell.</b>
</p>

## Content

- [Introduction](#introduction)
- [Built-in commands](#built-in-commands)
- [Signals](#signals)
- [Intallation](#installation)

## Introduction

In this project we made a small unix shell which imitates the behavior of bash. The shell supports pipes, redirections, environment variable expansions, a few built-in commands and user keyboard signals. The shell does not support `\`, `;`, `&&`, `||`, wildcards or parentheses.

The goal of this project is to learn how a unix shell works in a deeper level, how to parse input and execute shell commands in a separate process in C language. We implemented input tokenization, leveraging linked lists for token storage.

We were limited to use only the following library functions:\
`readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `add_history`, `malloc`, `free`, `write`, `access`, `open`, `read`, `close`, `fork`, `waitpid`, `sigaction`, `sigemptyset`, `exit`, `getcwd`, `chdir`, `stat`, `lstat`, `unlink`, `execve`, `dup2`, `pipe`, `strerror` and `perror`.

We were allowed to use Libft functions, that we had written in the start of the cursus, more information about that [here.](https://github.com/miladrahmat/Libft)

Since this was the first group project in the cursus, we learned how to use git for collaboration (branching, reviewing, merging etc.). We also learned to write the code in a more readable way, so that collaborating would be as efficient as possible.

## Built-in commands

For the project we implemented the following built-in commands:

| Command   | Description                                |
|-----------|--------------------------------------------|
| `cd`      | Change current working directory           |
| `echo`    | Print the arguments seperated by spaces    |
| `env`     | Print the contents of the environment      |
| `exit`    | Exit the shell with a specified exit code  |
| `export`  | Create and set environment variables       |
| `pwd`     | Print the current working directory        |
| `unset`   | Removes the specified environment variable |

## Signals

For the shell to work properly, we changed the actions taken when receiving the following signals:

| Signal name | Keyboard shortcut | Description when running a process                     | Description when not running a process |
|-------------|-------------------|--------------------------------------------------------|----------------------------------------|
| `SIGINT`    | `Ctrl + C`        | Terminates the process with `SIGINT`                   | Displays a new prompt on a new line    |
| `EOF`       | `Ctrl + D`        | Indicates that there's no more data to read from input | Exits the shell                        |
| `SIGQUIT`   | `Ctrl + \`        | Terminates the process with `SIGQUIT`                  | Does nothing                           |

## Installation

To install Minishell, run the following on the command line:

```
git clone https://github.com/miladrahmat/Minishell.git
cd Minishell
make
make clean
```

To use Minishell, simply run the program in the `Minishell` directory

```
./minishell
```
