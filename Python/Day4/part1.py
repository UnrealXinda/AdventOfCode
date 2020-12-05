import sys

def has_all_fields(fields):
    ALL_FIELDS = ["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"]
    return all(field in fields for field in ALL_FIELDS)

def get_field_name(kvp):
    return kvp.split(":")[0]

def get_field_names(line):
    return [get_field_name(kvp) for kvp in line.split(" ")]

def main():
    if len(sys.argv) != 2:
        print("Usage: %s [input_file]" % sys.argv[0])
        return
    
    input_file = sys.argv[1]
    valid_count = 0
    fields = []
    
    with open(input_file) as fp:
        for raw_line in fp:
            line = raw_line.rstrip()
            if not line:
                valid_count += 1 if has_all_fields(fields) else 0
                fields.clear()
            else:
                fields.extend(get_field_names(line))
            
    print("Total valid passport count: %d" % valid_count)
    

if __name__ == "__main__":
    main()
