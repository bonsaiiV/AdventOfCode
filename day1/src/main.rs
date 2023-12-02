use regex::Regex;
use std::fs::File;
use std::io::{self, BufRead};

fn main() {
    let input_file_name = "resources/input";
    let file = File::open(input_file_name).unwrap();
    let lines = io::BufReader::new(file).lines();

    let mut first_digit;
    let mut last_digit;
    let mut sum = 0;

    let re_string = r"(one|two|three|four|five|six|seven|eight|nine|\d)";
    let rre_string = r")one|two|three|four|five|six|seven|eight|nine|d\(";
    let rev_re_string = rre_string.chars().rev().collect::<String>();
    println!("{}", rev_re_string);
    //let re_digits = Regex::new(r"(\d)").unwrap();
    let re_digits = Regex::new(re_string).unwrap();
    let rev_re_digits = Regex::new(&rev_re_string).unwrap();

    for line in lines {
        
        if let Ok(line_str) = line{
            first_digit = 11;
            last_digit = 0;
            println!("{}", line_str);

            for cap in re_digits.captures_iter(&line_str).take(1) {
                let (_, [digit_str]) = cap.extract();
                //print!("{} = ", digit_str);
                first_digit = get_digit(digit_str);
                /*print!("{}, ", digit);
                    last_digit = digit;
                    if first_digit == 11 {
                        first_digit = digit;
                    }
                */
            }
            let rev_line_str = line_str.chars().rev().collect::<String>();
            println!("{}", rev_line_str);
            for cap in rev_re_digits.captures_iter(&rev_line_str).take(1) {
                let (_, [digit_str]) = cap.extract();
                let rev_digit_str = digit_str.chars().rev().collect::<String>();
                last_digit = get_digit(&rev_digit_str);
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
