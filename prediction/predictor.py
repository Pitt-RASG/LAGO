#!/usr/bin/env python3
import sys

def main():
    flag = True
    while flag:
        try:
            hello = input()
            print("12")

        except EOFError:
            flag = False

if __name__ == "__main__":
    main()
