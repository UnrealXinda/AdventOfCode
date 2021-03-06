import sys

def binary_search(str, start_idx, end_idx, lower_flag, upper_flag):
    min = 0
    max = (1 << end_idx - start_idx) - 1
    
    for i in range(start_idx, end_idx):
        if str[i] == lower_flag:
            max -= ((max - min) >> 1) + 1
        elif str[i] == upper_flag:
            min += ((max - min) >> 1) + 1

    assert min == max
    return min

def main():
    if len(sys.argv) != 2:
        print("Usage: %s [input_file]" % sys.argv[0])
        return

    SEAT_STR_LEN = 10
    ROW_STR_LEN = 7
    COL_STR_LEN = SEAT_STR_LEN - ROW_STR_LEN
    COL_SEAT_COUNT = 1 << COL_STR_LEN
    input_file = sys.argv[1]
    max_id = -1
    
    with open(input_file) as fp:
        for raw_line in fp:
            line = raw_line.rstrip()
            row = binary_search(line, 0, ROW_STR_LEN, 'F', 'B')
            col = binary_search(line, ROW_STR_LEN, SEAT_STR_LEN, 'L', 'R')
            max_id = max(max_id, row * COL_SEAT_COUNT + col)
            
    print("Max seat ID is: %d" % max_id)
    

if __name__ == "__main__":
    main()
