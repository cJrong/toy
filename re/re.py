# coding=utf-8
# author=veficos

import string

"""
re: or
  ;

or: concat '|' or
  | concat
  ;

concat: star concat
      | star
      ;

star: atom '*'
    | atom
    ;

atom:
    | char
    | '(' re ')'
    ;
"""

RE_ATOM     = 1
RE_CONCAT   = 2
RE_OR       = 3
RE_STAR     = 4
RE_PRIMARY  = 5
RE_END      = 6

class Node(object):
    def __init__(self, typ, expr=None, expr2=None):
        self.typ = typ
        self.left = expr
        self.right = expr2

    def __str__(self):
        if self.typ == RE_ATOM:
            return str(self.left)
        elif self.typ == RE_CONCAT:
            return str(self.left) + '.' + str(self.right)
        elif self.typ == RE_OR:
            return str(self.left) + '|' + str(self.right)
        elif self.typ == RE_STAR:
            return str(self.left) + '*'
        elif self.typ == RE_PRIMARY:
            return '(' + str(self.left) + ')'
        return '$'

class Parser(object):
    def __init__(self, expr):
        self.re = expr
        self.cursor = 0

    def parse(self):
        node = Node(typ=RE_END)
        while True:
            left = self.parse_concat()
            if left.typ == RE_END:
                return node
            elif self.cursor < len(self.re) and self.re[self.cursor] == '|':
                self.cursor += 1
                right = self.parse()
                if right.typ == RE_END:
                    raise Exception('expect a re in after "|"')
                node = Node(typ=RE_OR, expr=left, expr2=right)
            else:
                node = left
        return node

    def parse_concat(self):
        left = Node(typ=RE_END)
        while True:
            right = self.parse_star()
            if right.typ == RE_END:
                break
            left = Node(typ=RE_CONCAT, expr=left, expr2=right) if left.typ != RE_END else right
        return left

    def parse_star(self):
        node = self.parse_atom()

        if node.typ != RE_END and self.cursor < len(self.re) and self.re[self.cursor] == '*':
            self.cursor += 1
            return Node(typ=RE_STAR, expr=node)

        return node

    def parse_atom(self):
        if self.cursor >= len(self.re):
            return Node(typ=RE_END)
        elif self.re[self.cursor] in (string.ascii_letters + string.digits):
            node = Node(typ=RE_ATOM, expr=self.re[self.cursor])
            self.cursor += 1
            return node
        elif self.re[self.cursor] == '(':
            self.cursor += 1
            node = self.parse()
            if self.cursor >= len(self.re) or self.re[self.cursor] != ')':
                raise Exception('expect a ")"')
            self.cursor += 1
            return Node(typ=RE_PRIMARY, expr=node)
        return Node(typ=RE_END)

def compile(re):
    return Parser(re).parse()

def match(node, s, begin, end):
    if node.typ == RE_ATOM:
        if node.left == s[end:end+1]:
            return begin, end+1
    elif node.typ == RE_CONCAT:
        result = match(node.left, s, begin, end)
        if result:
            begin, end = result
            result2 = match(node.right, s, begin, end)
            if result2:
                return result2
    elif node.typ == RE_OR:
        result = match(node.left, s, begin, end)
        if result:
            return result
        result = match(node.right, s, begin, end)
        if result:
            return result
    elif node.typ == RE_PRIMARY:
        return match(node.left, s, begin, end)
    elif node.typ == RE_STAR:
        last = begin
        while True:
            result = match(node.left, s, last, end)
            if not result:
                break
            end = result[1]
            last += result[1]
        return None if begin == end else (begin, end)
    return None

def search(tree, s):
    result = []
    last_begin, last_end = 0, 0
    while True:
        matched = match(tree, s, 0, 0)
        if matched:
            begin, end = matched
            result.append((s[begin:end], (last_begin+begin, last_end+end)))
            last_begin += end
            last_end += end
            s = s[end:]
        else:
            if not s:
                break
            s = s[1:]
            last_begin += 1
            last_end += 1
    return result

if __name__ == '__main__':
    print(search(compile(r'c|d*'), 'cddddddfcukcdcd'))

    print(search(compile(r'cd*'), 'cddddddfcukcdcd'))

    print(search(compile(r'(cd)*'), 'cddddddfuckcdcd'))

    print(search(compile(r'fuck'), 'cddddddfuckcdcd'))
