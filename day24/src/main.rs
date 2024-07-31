use regex::Regex;

use std::fs::File;
use std::io::{self, BufRead};
//mod part1;
mod part2;
mod vector;
use crate::vector::V3;
//use num::integer::sqrt;

/*type V3 = vector::V3<f64>;
impl V3 {
    fn abs(&self) -> f64 {
        (self.x * self.x + self.y * self.y + self.z * self.z).sqrt()
    }
}*/
#[derive(Debug, Clone, PartialEq, Copy)]
struct Storm<T: Copy> {
    support: V3<T>,
    direction: V3<T>,
}
fn main() {
    let re_line = Regex::new(r"(-?\d+), (-?\d+), (-?\d+) @ (-?\d+), (-?\d+), (-?\d+)").unwrap(); //\@\w(\d+),\w(\d+),\w(\d+)").unwrap();
    let input_reader = io::BufReader::new(File::open("resources/input").unwrap());
    let mut float_storms = Vec::new();
    let mut i_storms = Vec::new();
    for line_opt in input_reader.lines() {
        let line = line_opt.unwrap();
        let line_cap = re_line.captures(&line).unwrap();
        let float_nums = line_cap
            .iter()
            .skip(1)
            .map(|cap| cap.unwrap().as_str().parse::<f64>().unwrap())
            .collect::<Vec<_>>();
        let float_hail = Storm {
            support: V3 {
                x: float_nums[0],
                y: float_nums[1],
                z: float_nums[2],
            },
            direction: V3 {
                x: float_nums[3],
                y: float_nums[4],
                z: float_nums[5],
            },
        };
        float_storms.push(float_hail);
        let i_nums = line_cap
            .iter()
            .skip(1)
            .map(|cap| cap.unwrap().as_str().parse::<isize>().unwrap())
            .collect::<Vec<_>>();
        let i_hail: Storm<isize> = Storm {
            support: V3 {
                x: i_nums[0],
                y: i_nums[1],
                z: i_nums[2],
            },
            direction: V3 {
                x: i_nums[3],
                y: i_nums[4],
                z: i_nums[5],
            },
        };
        i_storms.push(i_hail);
    }
    part2::i_pigeonhole_solve(&i_storms);
    //println!("{}", part2::get_idir_score(&i_storms, V3 { x: 44, y: 305, z: 75 }));
    //part1::solve(&hail_storms);
    //part2::bruteforce_direction(&float_storms);
    //println!("{}", get_dir_score(&hail_storms, V3{x:(-3).try_into().unwrap(), y:1.try_into().unwrap(), z:2.try_into().unwrap()}));
}
