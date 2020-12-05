import sys

def count_encountered_trees(landscape, step_x, step_y, tree_ch = '#'):
    count = 0
    x = 0
    for line in landscape[::step_y]:
        count += 1 if line[x % len(line)] == tree_ch else 0
        x += step_x
    return count

def main():
    if len(sys.argv) != 2:
        print("Usage: %s [input_file]" % sys.argv[0])
        return

    steps = [[1, 1],[3, 1],[5, 1],[7, 1],[1, 2]]
    tree_ch = '#'
    input_file = sys.argv[1]
    product = 1
    landscape = []
    
    with open(input_file) as fp:
        landscape = [line.rstrip() for line in fp]

    for step in steps:
        product *= count_encountered_trees(landscape, step[0], step[1])
            
    print("Total trees encountered: %d" % product)    

if __name__ == "__main__":
    main()
