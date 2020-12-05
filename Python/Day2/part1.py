import sys
import re

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
                min = int(match.group(1))
                max = int(match.group(2))
                ch = match.group(3)
                pwd = match.group(4)
                char_count = pwd.count(ch)
                valid_count += 1 if char_count >= min and char_count <= max else 0

    print("Total valid password count: %d" % valid_count)
    

if __name__ == "__main__":
    main()
