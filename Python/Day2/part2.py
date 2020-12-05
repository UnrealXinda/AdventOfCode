import sys
import re

def str_contains(string, ch, pos):
    return pos < len(string) and pos >= 0 and ch == string[pos]

def main():
    if len(sys.argv) != 2:
        print("Usage: %s [input_file]" % sys.argv[0])
        return
        
    input_file = sys.argv[1]
    valid_count = 0
    pattern = re.compile("^(\\d+)-(\\d+) ([a-z]) ([a-z]+)$")
    
    with open(input_file) as fp:
        for raw_line in fp:
            line = raw_line.rstrip()
            match = pattern.search(line)
            if match:
                first = int(match.group(1))
                second = int(match.group(2))
                ch = match.group(3)
                pwd = match.group(4)
                
                valid_count += 1 if str_contains(pwd, ch, first - 1) ^ str_contains(pwd, ch, second - 1) else 0

    print("Total valid password count: %d" % valid_count)
    

if __name__ == "__main__":
    main()
