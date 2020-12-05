import sys

def main():
    if len(sys.argv) != 2:
        print("Usage: %s [input_file]" % sys.argv[0])
        return

    x = 0
    x_step = 3
    tree_ch = '#'
    input_file = sys.argv[1]
    tree_count = 0
    
    with open(input_file) as fp:
        for raw_line in fp:
            line = raw_line.rstrip()
            tree_count += 1 if line[x % len(line)] == tree_ch else 0
            x += x_step
            
    print("Total trees encountered: %d" % tree_count)
    

if __name__ == "__main__":
    main()
