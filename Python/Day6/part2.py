import sys

def aggregate_answer(question_arr, answers):
    for ch in answers:
        question_arr[ord(ch) - ord('a')] += 1

def get_intersection_count(question_arr, person_count):
    return question_arr.count(person_count)

def main():
    if len(sys.argv) != 2:
        print("Usage: %s [input_file]" % sys.argv[0])
        return

    input_file = sys.argv[1]
    question_arr = [0] * 26
    person_count = 0
    count = 0
    
    with open(input_file) as fp:
        for raw_line in fp:
            line = raw_line.rstrip()
            if not line:
                count += get_intersection_count(question_arr, person_count)
                question_arr = [0] * 26
                person_count = 0
            else:
                aggregate_answer(question_arr, line)
                person_count += 1

    count += get_intersection_count(question_arr, person_count)   
    print("Total intersected questions answered: %d" % count)
    

if __name__ == "__main__":
    main()
