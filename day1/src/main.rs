use regex::Regex;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn main() {
    let input_file_name = "ressources/input1";
    let file = File::open(input_file_name).unwrap();
    let lines = io::BufReader::new(file).lines();

    let mut first_digit = 11;
    let mut last_digit = 0;
    let mut sum = 0;

    let re_digits = Regex::new(r"(one|two|three|four|five|six|seven|eight|nine|\d)").unwrap();

    for line in lines {
        
        if let Ok(line_str) = line{
            first_digit = 11;
            last_digit = 0;
            println!("{}", line_str);

            for cap in re_digits.captures_iter(&line_str) {
                let (_,[digit_str]) = cap.extract();
                print!("{} = ", digit_str);
                let digit = get_digit(digit_str);
                print!("{}, ", digit);
                    last_digit = digit;
                    if first_digit == 11 {
                        first_digit = digit;
                    }
                
            }
            println!("\n{}{}", first_digit,last_digit);
            if first_digit != 11 {
                sum += first_digit * 10 + last_digit;
            }
        }
    }
    println!("{sum}");
}

fn get_digit(digit_str: &str) -> u32 {
    match digit_str {
        "one" | "1" => 1,
        "two" | "2" => 2,
        "three" | "3" => 3,
        "four" | "4" => 4,
        "five" | "5" => 5,
        "six" | "6" => 6,
        "seven" | "7" => 7,
        "eight" | "8" => 8,
        "nine" | "9" => 9,
        _ => 0,
    }
}
