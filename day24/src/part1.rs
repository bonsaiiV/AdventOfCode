use crate::Storm;
use std::iter::repeat;

const RANGE_MIN: isize = 200000000000000;
const RANGE_MAX: isize = 400000000000000;

fn solve(hail_storms: &Vec<Storm>) {
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
            let delta =
                (storm2.support.x - storm1.support.x) / (storm1.direction.x - storm2.direction.x);
            let crossing_at = storm1.support + storm1.direction * delta;
            (
                crossing_at,
                (s + delta, t * delta),
                format!(
                    "{}\nhave same y at ({:?},{:?}) and meet at {:?}",
                    record, storm1, storm2, crossing_at
                ),
            )
        })
        .filter(|(pos, (s, t), _record)| {
            *s >= 0
                && *t >= 0
                && RANGE_MIN <= pos.x
                && RANGE_MAX >= pos.x
                && RANGE_MIN <= pos.y
                && RANGE_MAX >= pos.y
        })
        .count()
        / 2;
    println!("{:?}", result1);
}
