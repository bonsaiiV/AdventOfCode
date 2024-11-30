use regex::Regex;
use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead};

fn main() {
    let max_amount = HashMap::from([
        ("red", 12),
        ("green", 13),
        ("blue", 14),
    ]);
    let re = Regex::new(r" (\d+) (blue)| (\d+) (green)| (\d+) (red)").unwrap();
    //let re2 = Regex::new(r" (\d+) (blue|green|red)"
    let input_file = File::open("resources/input").unwrap();
    let lines = io::BufReader::new(input_file).lines();
    let mut sum = 0;
    let mut total_req = 0;
    let mut lineNr = 1;
    let mut bust; 
    for line in lines {
        let mut req_amount = HashMap::from([
            ("red", 0),
            ("green", 0),
            ("blue", 0),
        ]);

        bust = false;
        if let Ok(line_str) = line {
            for (_, [amount_str, colour]) in re.captures_iter(&line_str).map(|c| c.extract()){
                let amount = amount_str.parse::<usize>().unwrap();
                if  amount > *max_amount.get(colour).unwrap() {
                    bust = true;
                }
                if *req_amount.get(colour).unwrap() < amount {req_amount.insert(colour, amount);}
            }
            if !bust {sum += lineNr;}
            total_req += req_amount.values().product::<usize>();
            lineNr += 1;
        }
    }
    println!("{}, {}", sum, total_req);
}

