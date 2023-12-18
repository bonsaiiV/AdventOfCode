use regex::Regex;
use std::collections::HashMap;
use std::fs;

fn main() {
    let re = Regex::new(r"^([a-z]+)(\-|\=\d)").unwrap();
    let mut input_string = fs::read_to_string("resources/input").unwrap();
    input_string.pop();
    println!(
        "{:?}",
        input_string
            .split(',')
            .map(|word| hash_word(&word.to_string()))
            .sum::<usize>()
    );
    let mut boxes: HashMap<usize, Vec<(String, usize)>> = HashMap::new();
    input_string.split(',').for_each(|word| {
        let (_, [op_label, op]) = re.captures(word).unwrap().extract();
        let box_nr = hash_word(&op_label.to_string());
        match op {
            "-" => {
                if let Some(lens_box) = boxes.get_mut(&box_nr) {
                    lens_box.retain(|(lens_label, _)| lens_label != op_label)
                }
            }
            digit_str => {
                let digit = digit_str.to_string().pop().unwrap().to_digit(10).unwrap() as usize;
                if let Some(lens_box) = boxes.get_mut(&box_nr) {
                    if let Some(pos) = lens_box
                        .iter_mut()
                        .position(|(lens_label, _)| lens_label == op_label)
                    {
                        let lens = lens_box.get_mut(pos).unwrap();
                        lens.1 = digit;
                    } else {
                        lens_box.push((op_label.to_string(), digit))
                    }
                } else {
                    boxes.insert(box_nr, Vec::from([(op_label.to_string(), digit)]));
                }
            }
        }
    });
    println!(
        "{}",
        (0..256)
            .map(|box_nr| {
                (box_nr + 1)
                    * boxes.get(&box_nr).map_or(0, |lens_box| {
                        lens_box
                            .iter()
                            .enumerate()
                            .fold(0, |sum, (pos, (_, focal_length))| {
                                sum + (pos + 1) * focal_length
                            })
                    })
            })
            .sum::<usize>()
    );
}
fn hash_word(word: &String) -> usize {
    word.chars().fold(0, |hash, sym| next_hash(hash, sym))
}
fn next_hash(hash: usize, sym: char) -> usize {
    (hash + sym as usize) * 17 % 256
}
