import sys

def main():
    if len(sys.argv) != 3:
        print("Usage: %s [input_file] [total_sum]" % sys.argv[0])
        return
        
    input_file = sys.argv[1]
    total_sum = int(sys.argv[2])
    nums = []
    hash_set = set()
    
    with open(input_file) as fp:
        for line in fp:
            nums.append(int(line))

    for i in range(0, len(nums)):
        a = nums[i]
        b = total_sum - a
        if b in hash_set:
            print("%d + %d = %d" % (a, b, total_sum))
            print("%d * %d = %d" % (a, b, a * b))
            return
        hash_set.add(a)

    print(total_sum)
    

if __name__ == "__main__":
    main()
