import sys
import re

def is_valid_birth_year(value):
    year = int(value)
    return year >= 1920 and year <= 2002

def is_valid_issue_year(value):
    year = int(value)
    return year >= 2010 and year <= 2020

def is_valid_expiration_year(value):
    year = int(value)
    return year >= 2020 and year <= 2030

def is_valid_height(value):
    pos = value.find("cm")
    if pos >= 0:
        height = int(value[0:pos])
        return height >= 150 and height <= 193
    else:
        pos = value.find("in")
        if pos >= 0:
            height = int(value[0:pos])
            return height >= 59 and height <= 76
    return False

def is_valid_hair_color(value):
    hair_color = re.compile("^#[a-f0-9]{6}$")
    return hair_color.search(value)

def is_valid_eye_color(value):
    eye_colors = ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"]
    return any(value == color for color in eye_colors)

def is_valid_passport_id(value):
    passport_id_format = re.compile("^(\\d){9}$")
    return passport_id_format.search(value)

def is_valid_field(key, value):
    if key == "byr":
        return is_valid_birth_year(value)
    elif key == "iyr":
        return is_valid_issue_year(value)
    elif key == "eyr":
        return is_valid_expiration_year(value)
    elif key == "hgt":
        return is_valid_height(value)
    elif key == "hcl":
        return is_valid_hair_color(value)
    elif key == "ecl":
        return is_valid_eye_color(value)
    elif key == "pid":
        return is_valid_passport_id(value)
    else:
        return True        

def has_all_fields(fields):
    ALL_FIELDS = ["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"]
    return all(field in fields for field in ALL_FIELDS)

def get_key_value_pair(value):
    kvp = value.split(":")
    return kvp[0], kvp[1]

def get_key_value_pairs(line):
    return [get_key_value_pair(substr) for substr in line.split(" ")]

def is_valid_passport(kvps):
    return has_all_fields([key for [key, value] in kvps]) and all(is_valid_field(key, value) for [key, value] in kvps)

def main():
    if len(sys.argv) != 2:
        print("Usage: %s [input_file]" % sys.argv[0])
        return
    
    input_file = sys.argv[1]
    valid_count = 0
    kvps = []
    
    with open(input_file) as fp:
        for raw_line in fp:
            line = raw_line.rstrip()
            if not line:
                valid_count += 1 if is_valid_passport(kvps) else 0
                kvps.clear()
            else:
                kvps.extend(get_key_value_pairs(line))
            
    print("Total valid passport count: %d" % valid_count)
    

if __name__ == "__main__":
    main()
