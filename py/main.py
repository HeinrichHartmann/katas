#!/usr/bin/env python3
from pprint import pprint
import math

class ListNode(object):
    def __init__(self, val=0, next=None):
       self.val = val
       self.next = next

    @classmethod
    def fromNumber(cls, num):
        root = None
        while num > 0:
            val = num % 10
            num = num // 10
            old_root = root
            root = cls(val, old_root)
        return root.reverse()

    def reverse(self):
        stack = []
        head = self
        while head != None:
            stack.append(head)
            head = head.next

        out = stack[-1]
        while len(stack) > 0:
            node = stack.pop()
            node.next = stack[-1] if len(stack) > 0 else None

        return out

    def toNumber(self):
        exp = 1
        head = self
        acc = 0
        while head != None:
            acc = acc + head.val * exp
            exp *= 10
            head = head.next
        return acc

    def __repr__(self):
        return f"{[self.val]}->{self.next}"

class Solution(object):

    def addTwoNumbers(self, l1, l2):
        """
        :type l1: Optional[ListNode]
        :type l2: Optional[ListNode]
        :rtype: Optional[ListNode]
        """
        out = None
        carry = 0
        while True:
            a = 0
            b = 0
            if l1 != None:
                a = l1.val
            if l2 != None:
                b = l2.val

            c = a + b + carry

            if c == 0:
                break

            val = c % 10
            carry = c // 10
            out = ListNode(val, out)

            if l1:
                l1 = l1.next
            if l2:
                l2 = l2.next

        return out.reverse()

def main():
    print("Hello world!")
    print("1234 -> ")
    pprint(ListNode.fromNumber(1234))
    a = ListNode.fromNumber(1)
    b = ListNode.fromNumber(1)
    pprint([a,b,a.toNumber(), b.toNumber()])
    s = Solution()
    pprint(s.addTwoNumbers(a,b).toNumber())

    a = ListNode.fromNumber(123)
    b = ListNode.fromNumber(123)
    pprint([a,b,a.toNumber(), b.toNumber()])
    s = Solution().addTwoNumbers(a,b)
    pprint([s,s.toNumber()])

    a = ListNode.fromNumber(9)
    b = ListNode.fromNumber(9)
    pprint([a,b,a.toNumber(), b.toNumber()])
    s = Solution().addTwoNumbers(a,b)
    pprint([s,s.toNumber()])

    a = ListNode.fromNumber(9)
    b = ListNode.fromNumber(9)
    pprint([a,b,a.toNumber(), b.toNumber()])
    s = Solution().addTwoNumbers(a,b)
    pprint([s,s.toNumber()])


if __name__ == "__main__":
    main()
