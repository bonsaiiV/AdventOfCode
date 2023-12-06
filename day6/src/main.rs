use regex::Regex;
use std::fs;

fn main() {
    let input_str = fs::read_to_string("resources/input").unwrap();
    let mut lines = input_str.lines();
    let re_nums = Regex::new(r"(\d+)").unwrap();
    let mut inputs = re_nums.captures_iter(&lines.next().unwrap())
        .map(|cap| cap.extract())
        .map(|(_,[time_str])| time_str.parse::<usize>().unwrap())
        .zip(re_nums.captures_iter(lines.next().unwrap())
        .map(|cap| cap.extract())
        .map(|(_,[time_str])| time_str.parse::<usize>().unwrap()));
    let mut result1 = 1;
    for (time, goal_distance) in inputs{
        result1 *= (0 .. time).into_iter().map(|t_hold| t_hold * (time-t_hold)).filter(|dist| dist > &goal_distance).count(); 
    }
    println!("{result1}");

    lines = input_str.lines();
    let time = re_nums.find_iter(&lines.next().unwrap()).fold(String::from(""), |a,b| format!("{}{}",a,b.as_str())).parse::<usize>().unwrap();
    let goal_distance = re_nums.find_iter(&lines.next().unwrap()).fold(String::from(""), |a,b| format!("{}{}",a,b.as_str())).parse::<usize>().unwrap();
        let result2 = (0 .. time).into_iter().map(|t_hold| t_hold * (time-t_hold)).filter(|dist| dist > &goal_distance).count(); 
    println!("{result2}");
}
