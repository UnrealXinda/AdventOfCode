import sys

def aggregate_answer(question_arr, answers):
    for ch in answers:
        question_arr[ord(ch) - ord('a')] = True

def main():
    if len(sys.argv) != 2:
        print("Usage: %s [input_file]" % sys.argv[0])
        return

    input_file = sys.argv[1]
    question_arr = [False] * 26
    count = 0
    
    with open(input_file) as fp:
        for raw_line in fp:
            line = raw_line.rstrip()
            if not line:
                count += question_arr.count(True)
                question_arr = [False] * 26
            else:
                aggregate_answer(question_arr, line)

    count += question_arr.count(True)      
    print("Total questions answered: %d" % count)
    

if __name__ == "__main__":
    main()
