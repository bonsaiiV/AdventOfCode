use std::fs::File;
use std::io::{self, BufRead};

fn main() {
    let read_input = io::BufReader::new(File::open("resources/input").unwrap());
    let hands = read_input.lines().map(|line| {
        let line_str = line.unwrap();
        let (hand, points) = line_str.split_once(' ').unwrap();
        (hand.to_owned(), points.parse::<usize>().unwrap())
    });
    let mut hands_vec = hands.collect::<Vec<(String,usize)>>();
    hands_vec.sort_by_cached_key(|(hand,_)| _get_score2(hand));
    let result1: usize = hands_vec.iter().enumerate().map(|(index, (_,points))| (index + 1)*points).sum();
    println!("{}",result1);
}


fn _get_score(hand: &String) -> usize{
    println!("{hand}");
    let mut counts = std::iter::repeat(0).take(14).collect::<Vec<usize>>();
    let mut hand_score = hand.chars().map(to_index).collect::<Vec<usize>>();
    hand.chars().fold(&mut counts, |counts_f, next_char| {counts_f[to_index(next_char)] +=1; counts_f});
    let joker = counts.remove(0);
    let mut indexes = (0..counts.len()).collect::<Vec<usize>>();
    indexes.sort_by_key(|i| counts.get(*i).unwrap());
    let index_of_max = indexes.pop().unwrap();
    counts[index_of_max] += joker;
    if counts.contains(&5){hand_score.insert(0,6)}
    else if counts.contains(&4) {hand_score.insert(0,5)}
    else if counts.contains(&3) && counts.contains(&2) {hand_score.insert(0,4)}
    else if counts.contains(&3) {hand_score.insert(0,3)}
    else if counts.iter().filter(|c| **c==2).count() ==2{hand_score.insert(0,2)
    }
    else if counts.contains(&2) {hand_score.insert(0,1)}
    else {hand_score.insert(0,0)}
    let score_of_hand = hand_score.iter().fold(0, |score, next_part| score*100+next_part);
    println!("{score_of_hand}");
    score_of_hand
}

fn to_index(card: char) -> usize{
    match card {
        'A' => 12,
        'K' => 11,
        'Q' => 10,
        'J' => 0,
        'T' => 9,
        digit => <u32 as TryInto<usize>>::try_into(digit.to_digit(10).unwrap()).unwrap()-1,
    }
}
//rewrite
fn _get_score2(hand: &String) -> usize{
    let mut counts = std::iter::repeat(0).take(14).collect::<Vec<usize>>();
    let hand_score = hand.chars().map(to_index).collect::<Vec<usize>>();
    hand.chars().fold(&mut counts, |counts_f, next_char| {counts_f[to_index(next_char)] +=1; counts_f});
    let count_of_joker = counts.remove(0);
    counts.sort();
    let score_of_hand = get_hand_type((count_of_joker + counts.pop().unwrap(),counts.pop().unwrap()));
    hand_score.iter().fold(score_of_hand, |score, next_part| score*100+next_part)
}

fn get_hand_type(highest_kinds: (usize, usize)) -> usize {
    match highest_kinds {
        (5, _) => 6,
        (4, _) => 5,
        (3, 2) => 4,
        (3, _) => 3,
        (2, 2) => 2,
        (2, _) => 1,
        _ => 0,
    }
}
