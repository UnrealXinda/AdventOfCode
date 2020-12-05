import sys

def getSumPair(arr, target_sum, start, end):
    hash_set = set()
    for i in range(start, end):
        a = arr[i]
        b = target_sum - a
        if b in hash_set:
            return True, a, b
        hash_set.add(a)

    return False, None, None

def main():
    if len(sys.argv) != 3:
        print("Usage: %s [input_file] [total_sum]" % sys.argv[0])
        return
        
    input_file = sys.argv[1]
    total_sum = int(sys.argv[2])
    nums = []
    
    with open(input_file) as fp:
        for line in fp:
            nums.append(int(line))

    for i in range(0, len(nums)):
        a = nums[i]
        target_sum = total_sum - a
        found, b, c = getSumPair(nums, target_sum, i + 1, len(nums))
        if found:
            print("%d + %d + %d = %d" % (a, b, c, total_sum))
            print("%d * %d * %d = %d" % (a, b, c, a * b * c))
            return
        

if __name__ == "__main__":
    main()
