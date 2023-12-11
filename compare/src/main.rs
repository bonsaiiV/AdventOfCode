use regex::Regex;
use std::collections::HashSet;

use std::io::{self,BufRead};
use std::fs::File;

fn main() {
    let re_tuples = Regex::new(r"\((\d+), (\d+)\)").unwrap();
    let re_arr = Regex::new(r"\[(\d+), (\d+), (\d+)\]").unwrap();

    let mut lines1 = io::BufReader::new(File::open("resources/out1").unwrap()).lines();
    let mut lines2 = io::BufReader::new(File::open("resources/out2").unwrap()).lines();
    let mut line_nr = 1;
    while let Some(line1) = lines1.next(){
        let line1_str = line1.unwrap();
        if let Some(line2) = lines2.next(){
            let line2_str = line2.unwrap();
            let mut line2_tuples = re_tuples.captures_iter(&line2_str)
                .map(|cap| cap.extract())
                .map(|(_, [start, end])| (start.parse::<usize>().unwrap(), end.parse::<usize>().unwrap()))
            .collect::<HashSet<(usize,usize)>>();
            let mut line2_arrs = re_arr.captures_iter(&line2_str)
                .map(|cap| cap.extract())
                .map(|(_, [dest, src, len])| (dest.parse::<usize>().unwrap(), src.parse::<usize>().unwrap(), len.parse::<usize>().unwrap()))
            .collect::<HashSet<(usize,usize,usize)>>();
            for tup in re_tuples.captures_iter(&line1_str)
                .map(|cap| cap.extract())
                .map(|(_, [start, end])| 
                    (start.parse::<usize>().unwrap(), end.parse::<usize>().unwrap())){
                if !line2_tuples.remove(&tup){
                    println!("{line_nr}: Tuple {:?} only found in File1", tup);
                }
            }
            for arr in re_arr.captures_iter(&line1_str)
                .map(|cap| cap.extract())
                .map(|(_, [dest, src, len])| (dest.parse::<usize>().unwrap(), src.parse::<usize>().unwrap(), len.parse::<usize>().unwrap())){
                if !line2_arrs.remove(&arr){
                    println!("{line_nr}: Tuple {:?} only found in File1", arr);
                }
            }
            println!("");
            for tup in line2_tuples{
                println!("{line_nr}: Tuple {:?} only found in File2", tup);
            }
            for arr in line2_arrs{
                println!("{line_nr}: Arr {:?} only found in File2", arr);
            }
            println!("");


        }else{
            println!("{line_nr} is only found in File1: {line1_str}"); 
        }
        line_nr += 1;
    }
    while let Some(line2) = lines2.next(){
        let line2_str = line2.unwrap();
        println!("{line_nr} is only found in File2: {line2_str}"); 
        line_nr += 1;
    }

}
