use regex::Regex;

use std::io::{self, Read};
use std::fs;


fn main() {
    let mut count_of_cards: Vec<usize> = std::iter::repeat(1).take(211).collect();
    let mut cardNr:i32 = 0;
    let mut sum1 = 0;
    let re = Regex::new(r"(\d+|\|)").unwrap();
    for line in fs::read_to_string("resources/input").unwrap().lines().rev(){
        let line_str = line;
        let mut all_matches = re.find_iter(&line_str);
        all_matches.nth(0);
        let win_numbers: Vec<&str> = all_matches.by_ref()
            .take_while(|mat| mat.as_str() != "|")
            .map(|mat| mat.as_str())
            .collect();
    
        let win_count:i32 = all_matches.filter(|nr| win_numbers.contains(&nr.as_str())).count().try_into().unwrap();
        let two: usize = 2;
        if win_count > 0 {
            sum1 += two.pow((win_count-1).try_into().unwrap());
        }
        print!("{}, ", win_count);
        let mut extra_cards = 0;
        for add_card in cardNr-win_count..cardNr{
            extra_cards += count_of_cards.get(TryInto::<usize>::try_into(add_card).unwrap_or(300)).unwrap_or(&0);
        }
        count_of_cards[TryInto::<usize>::try_into(cardNr-0).unwrap()] = extra_cards + 1;
        cardNr += 1;
    }
    println!("\n{:?}", count_of_cards);
    println!("{}\n{}", sum1, count_of_cards.iter().sum::<usize>());
}
