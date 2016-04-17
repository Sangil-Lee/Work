.. productionlist::
   program: "program" `identifier` `program_param` `global_defns` `entry` `state_sets` `exit` `c_codes`
   program_param: "(" `string` ")"
   program_param: 
   global_defns: `global_defns` `global_defn`
   global_defns: 
   global_defn: `assign`
   global_defn: `monitor`
   global_defn: `sync`
   global_defn: `syncq`
   global_defn: `declaration`
   global_defn: `option`
   global_defn: `c_code`
   assign: `connect` `variable` `to` `string` ";"
   assign: `connect` `variable` `subscript` `to` `string` ";"
   assign: `connect` `variable` `to` "{" `strings` "}" ";"
   assign: `connect` `variable` ";"
   connect: "assign"
   connect: "connect"
   to: "to"
   to: 
   strings: `strings` "," `string`
   strings: `string`
   strings: 
   monitor: "monitor" `variable` `opt_subscript` ";"
   sync: "sync" `variable` `opt_subscript` `to` `event_flag` ";"
   syncq: "syncq" `variable` `opt_subscript` `to` `event_flag` `syncq_size` ";"
   syncq: "syncq" `variable` `opt_subscript` `syncq_size` ";"
   event_flag: `identifier`
   variable: `identifier`
   syncq_size: `integer_literal`
   syncq_size: 
   opt_subscript: `subscript`
   opt_subscript: 
   subscript: "[" `integer_literal` "]"
   declaration: `type` `init_declarators` ";"
   init_declarators: `init_declarator`
   init_declarators: `init_declarators` "," `init_declarator`
   init_declarator: `declarator`
   init_declarator: `declarator` "=" `init_expr`
   declarator: "*" `declarator`
   declarator: `direct_declarator`
   direct_declarator: `variable`
   direct_declarator: "(" `declarator` ")"
   direct_declarator: `direct_declarator` `subscript`
   init_expr: "{" `init_exprs` "}"
   init_expr: `expr`
   init_exprs: `init_exprs` "," `init_expr`
   init_exprs: `init_expr`
   init_exprs: 
   type: "char"
   type: "short"
   type: "int"
   type: "long"
   type: "unsigned" "char"
   type: "unsigned" "short"
   type: "unsigned" "int"
   type: "unsigned" "long"
   type: "int8_t"
   type: "uint8_t"
   type: "int16_t"
   type: "uint16_t"
   type: "int32_t"
   type: "uint32_t"
   type: "float"
   type: "double"
   type: "string"
   type: "evflag"
   type: "foreign"
   option: "option" `option_value` `identifier` ";"
   option_value: "+"
   option_value: "-"
   state_sets: `state_sets` `state_set`
   state_sets: `state_set`
   state_set: "ss" `identifier` "{" `ss_defns` `states` "}"
   ss_defns: `ss_defns` `ss_defn`
   ss_defns: 
   ss_defn: `assign`
   ss_defn: `monitor`
   ss_defn: `sync`
   ss_defn: `syncq`
   ss_defn: `declaration`
   states: `states` `state`
   states: `state`
   state: "state" `identifier` "{" `state_defns` `entry` `whens` `exit` "}"
   state_defns: `state_defns` `state_defn`
   state_defns: 
   state_defn: `assign`
   state_defn: `monitor`
   state_defn: `sync`
   state_defn: `syncq`
   state_defn: `declaration`
   state_defn: `option`
   entry: "entry" `block`
   entry: 
   exit: "exit" `block`
   exit: 
   whens: `whens` `when`
   whens: `when`
   when: "when" "(" `opt_expr` ")" `block` "state" `identifier`
   when: "when" "(" `opt_expr` ")" `block` "exit"
   block: "{" `block_defns` `statements` "}"
   block_defns: `block_defns` `block_defn`
   block_defns: 
   block_defn: `declaration`
   block_defn: `c_code`
   statements: `statements` `statement`
   statements: 
   statement: "break" ";"
   statement: "continue" ";"
   statement: "state" `identifier` ";"
   statement: `c_code`
   statement: "{" `block_defns` `statements` "}"
   statement: "if" "(" `comma_expr` ")" `statement`
   statement: "if" "(" `comma_expr` ")" `statement` "else" `statement`
   statement: "while" "(" `comma_expr` ")" `statement`
   statement: `for_statement`
   statement: `opt_expr` ";"
   for_statement: "for" "(" `opt_expr` ";" `opt_expr` ";" `opt_expr` ")" `statement`
   expr: `integer_literal`
   expr: `floating_point_literal`
   expr: `string`
   expr: `variable`
   expr: "(" `comma_expr` ")"
   expr: `identifier` "(" `args` ")"
   expr: "exit" "(" `args` ")"
   expr: `expr` "[" `expr` "]"
   expr: `expr` "." `expr`
   expr: `expr` "->" `expr`
   expr: `expr` "++"
   expr: `expr` "--"
   expr: "+" `expr`
   expr: "-" `expr`
   expr: "*" `expr`
   expr: "&" `expr`
   expr: "!" `expr`
   expr: "~" `expr`
   expr: "++" `expr`
   expr: "--" `expr`
   expr: `expr` "-" `expr`
   expr: `expr` "+" `expr`
   expr: `expr` "*" `expr`
   expr: `expr` "/" `expr`
   expr: `expr` ">" `expr`
   expr: `expr` ">=" `expr`
   expr: `expr` "==" `expr`
   expr: `expr` "!=" `expr`
   expr: `expr` "<=" `expr`
   expr: `expr` "<" `expr`
   expr: `expr` "||" `expr`
   expr: `expr` "&&" `expr`
   expr: `expr` "<<" `expr`
   expr: `expr` ">>" `expr`
   expr: `expr` "|" `expr`
   expr: `expr` "^" `expr`
   expr: `expr` "&" `expr`
   expr: `expr` "%" `expr`
   expr: `expr` "?" `expr` ":" `expr`
   expr: `expr` "=" `expr`
   expr: `expr` "+=" `expr`
   expr: `expr` "-=" `expr`
   expr: `expr` "&=" `expr`
   expr: `expr` "|=" `expr`
   expr: `expr` "/=" `expr`
   expr: `expr` "*=" `expr`
   expr: `expr` "%=" `expr`
   expr: `expr` "<<=" `expr`
   expr: `expr` ">>=" `expr`
   expr: `expr` "^=" `expr`
   comma_expr: `comma_expr` "," `expr`
   comma_expr: `expr`
   opt_expr: `comma_expr`
   opt_expr: 
   args: `args` "," `expr`
   args: `expr`
   args: 
   string: `string_literal`
   c_codes: `c_codes` `c_code`
   c_codes: 
   c_code: `embedded_c_code`
