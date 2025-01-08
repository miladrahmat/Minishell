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

## Introduction

In this project we made a small unix shell which imitates the behavior of bash. We implemented pipes, redirections, environment variable expansions and a few built-in commands.

The goal of this project is to learn how a unix shell works in a deeper level, how to parse input and execute shell commands in a separate process in C language. We implemented input tokenization from scratch, leveraging linked lists for token storage.

## Built-in commands

For the project we implemented the following built-in commands:

| Command | Description                                |
|---------|--------------------------------------------|
| cd      | Change current working directory           |
| echo    | Print the arguments seperated by spaces    |
| env     | Print the contents of the environment      |
| exit    | Exit the shell with a specified exit code  |
| export  | Create and set environment variables       |
| pwd     | Print the current working directory        |
| unset   | Removes the specified environment variable |
