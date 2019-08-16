" Vim syntax file
" Language: OCC
" Maintainer: Robert Hrusecky
" Latest Revision: 2 July 2019

if exists("b:current_syntax")
  finish
endif

" Capital letter/Class types
syn match occType '[A-Z][A-Za-z0-9_]*'
hi def link occType        Type
syn match occIdentifier '[a-z_][A-Za-z0-9_]*'
hi def link occIdentifier Identifier

syn keyword occKeyword con mut new del intf type break ret fun class struct self namespace
hi def link occKeyword Keyword
syn keyword occBool true false
hi def link occBool Boolean
syn keyword occConditional if else
hi def link occConditional Conditional
syn keyword occLoop for while loop
syn keyword occLoop Repeat

" Operators
syn match occOperator "\v[-!%&*+/?|{}\[\]=\<\>(),_.:;]"
hi def link occOperator Operator

" Numbers
syn match occNumber '[+-]?[0-9]+'
hi def link occNumber Number

" Blocks
syn region occScope start="{" end="}" fold transparent
hi def link occScope Statement

" Strings
syn region occString start="\"" end="\""
hi def link occString String

" Comments
syn keyword occTodo contained TODO
hi def link occTodo Todo
syn match occComment "//.*$" contains=occTodo
hi def link occComment Comment

let b:current_syntax = "occ"

