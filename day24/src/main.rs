use regex::Regex;
use std::fs::File;
use std::io::{self, BufRead};
mod vector;
use crate::vector::V2;
use std::iter::repeat;

#[derive(Debug)]
struct Storm {
    support: V2,
    direction: V2,
}
fn main() {
    let re_line = Regex::new(r"(-?\d+), (-?\d+), (-?\d+) @ (-?\d+), (-?\d+), (-?\d+)").unwrap(); //\@\w(\d+),\w(\d+),\w(\d+)").unwrap();
    let input_reader = io::BufReader::new(File::open("resources/input").unwrap());
    let (range_max, range_min);
    {range_min = 200000000000000.0; range_max = 400000000000000.0;}
    /*{
        range_min = 7.0;
        range_max = 27.0;
    }*/
    let mut hail_storms = Vec::new();
    for line_opt in input_reader.lines() {
        let line = line_opt.unwrap();
        let line_cap = re_line.captures(&line).unwrap();
        let nums = line_cap
            .iter()
            .skip(1)
            .map(|cap| cap.unwrap().as_str().parse::<f64>().unwrap())
            .collect::<Vec<_>>();
        let hail = Storm {
            support: V2 {
                x: nums[0],
                y: nums[1],
            },
            direction: V2 {
                x: nums[3],
                y: nums[4],
            },
        };
        hail_storms.push(hail);
    }
    let result1 = hail_storms
        .iter()
        .flat_map(|storm_other| hail_storms.iter().zip(repeat(storm_other)))
        .filter(|(storm1, storm2)| storm1.direction != storm2.direction)
        .map(|(storm1, storm2)| {
            let s = (storm2.support.y - storm1.support.y) / storm1.direction.y;
            let t = storm1.direction.y / storm2.direction.y;
            (
                Storm {
                    support: storm1.support + storm1.direction * s,
                    direction: storm1.direction,
                },
                Storm {
                    support: storm2.support,
                    direction: storm2.direction * t,
                },
                (s, t),
                format!("{:?}, and {:?}", storm1, storm2),
            )
        })
        .map(|(storm1, storm2, (s, t), record)| {
            let delta = (storm2.support.x - storm1.support.x) / (storm1.direction.x - storm2.direction.x);
            let crossing_at =
                storm1.support
                    + storm1.direction
                        * delta;
            (
                crossing_at,
                (s + delta, t*delta),
                format!("{}\nhave same y at ({:?},{:?}) and meet at {:?}",record, storm1, storm2, crossing_at),
            )
        })
        .filter(|(pos, (s,t), record)| {
            println!("{}\nwith s = {} and t = {}.\n", record, s, t);
            *s >= 0.0 && *t >= 0.0 &&
            range_min <= pos.x && range_max >= pos.x && range_min <= pos.y && range_max >= pos.y
        })
        .count()/2;
    println!("{:?}", result1);
}
