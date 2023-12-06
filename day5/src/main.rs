use regex::Regex;
use std::collections::HashSet;

use std::fs::File;
use std::io::{self, BufRead};

fn main() {
    let re_nums = Regex::new(r"\d+").unwrap();
    let re_map = Regex::new(r"map").unwrap();
    let re_capture = Regex::new(r"(\d+)\s(\d+)\s(\d+)").unwrap();
    let mut lines = io::BufReader::new(File::open("resources/input").unwrap()).lines();
    let seed_line = lines.next().unwrap().unwrap();
    let mut seed_iter = re_nums
        .find_iter(&seed_line)
        .map(|mat| mat.as_str().parse::<usize>().unwrap());
    let mut seed_ranges: HashSet<(usize, usize)> = HashSet::new();
    while let Some(next_seed) = seed_iter.next() {
        seed_ranges.insert((next_seed, next_seed + seed_iter.next().unwrap()));
    }
    let mut current_vals: HashSet<(usize, usize)> = seed_ranges;
    let mut next_vals: HashSet<(usize, usize)>;
    let mut next_map = HashSet::new();
    for line in lines {
        let line_str = line.unwrap();
        if re_map.is_match(&line_str) {
            println!("{}", line_str);
            next_vals = map_to_next(current_vals, &next_map);
            current_vals = next_vals;
            next_map = HashSet::new();
        } else if let Some((_, nums_arr)) =
            re_capture.captures(&line_str).map(|cap| cap.extract::<3>())
        {
            let nums: Vec<usize> = nums_arr
                .iter()
                .map(|str| str.parse::<usize>().unwrap())
                .collect();
            next_map.insert(nums);
        }
    }
    println!(
        "{:?}",
        map_to_next(current_vals, &next_map).iter().min_by_key(|(start, _end)| start).unwrap());
}

fn map_to_next(val: HashSet<(usize, usize)>, next_map: &HashSet<Vec<usize>>) -> HashSet<(usize, usize)> {
    let mut not_yet_mapped = val;
    let mut ret: HashSet<(usize, usize)> = HashSet::new();
    println!("{:?}", not_yet_mapped);
    for region in next_map.iter() {
        print!("{:?}, ", region);
        let mut tmp: HashSet<(usize, usize)> = HashSet::new();
        for (start, end) in not_yet_mapped.clone().iter() {
            if region[1] <= *start && region[1] + region[2] >= *end {
                ret.insert((*start + region[0] - region[1], *end + region[0] - region[1]));
            } else if region[1] < *end && region[1] + region[2] >= *end {
                ret.insert((region[0], *end + region[0] - region[1]));
                tmp.insert((*start, region[1]));
            } else if region[1] <= *start && region[1] + region[2] > *start {
                ret.insert((*start + region[0] - region[1], region[0] + region[2]));
                tmp.insert((region[1] + region[2], *end));
            } else if region[1] > *start && region[1] + region[2] < *start {
                ret.insert((region[0], region[0] + region[2]));
                tmp.insert((region[1] + region[2], *end));
                tmp.insert((*start, region[1]));
            } else {
                tmp.insert((*start, *end));
            }
        }
        not_yet_mapped = tmp;
    }
    println!("");
    ret.extend(not_yet_mapped.iter());
    for (start,end) in ret.iter(){
        if start > end {
            println!("Bad range: ({start},{end})");
        }
    }
    ret
}
