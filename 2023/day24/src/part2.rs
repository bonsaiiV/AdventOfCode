use crate::vector::V3;
use crate::Storm;
use num::integer::{gcd, lcm};
use std::iter::repeat;
use std::io::{self, Write};

fn possible_projections(support: isize, direction: isize, projector: isize) -> Vec<isize> {
    let mut new_support = support.rem_euclid(projector);
    let mut possible_projections = vec![new_support];
    loop {
        new_support = (new_support + direction).rem_euclid(projector);
        if possible_projections
            .iter()
            .any(|&projection| projection == new_support)
        {
            break;
        } else {
            possible_projections.push(new_support);
        }
    }
    possible_projections
}

fn any_valid_projection(projections: &Vec<Vec<isize>>) -> bool{
    let possible_projections = projections.get(0).unwrap();
    possible_projections
        .iter()
        .filter(|&offset| {
            projections.iter().all(|storm_projections| {
                storm_projections
                    .iter()
                    .any(|&projection| projection == *offset)
            })
        })
        .count()
    > 0
}
pub fn i_pigeonhole_solve(storms: &Vec<Storm<isize>>) {
    let x_projectors = (1..400)
        .into_iter()
        .filter(|candidate_projector| {
            let projections = storms
                .iter()
                .map(|storm| {
                    possible_projections(storm.support.x, storm.direction.x, *candidate_projector)
                })
                .collect::<Vec<_>>();
            any_valid_projection(&projections)
        })
        .collect::<Vec<_>>();
    let nr_of_x_projectors = x_projectors.len();
    println!("#x-projectors: {}", nr_of_x_projectors);
    let y_projectors = (1..400)
        .into_iter()
        .filter(|candidate_projector| {
            let projections = storms
                .iter()
                .map(|storm| {
                    possible_projections(storm.support.y, storm.direction.y, *candidate_projector)
                })
                .collect::<Vec<_>>();
            any_valid_projection(&projections)
        })
        .collect::<Vec<_>>();
    println!("#y-projectors: {}", y_projectors.len());
    let z_projectors = (1..400)
        .into_iter()
        .filter(|candidate_projector| {
            let projections = storms
                .iter()
                .map(|storm| {
                    possible_projections(storm.support.z, storm.direction.z, *candidate_projector)
                })
                .collect::<Vec<_>>();
            any_valid_projection(&projections)
        })
        .collect::<Vec<_>>();
    println!("#z-projectors: {}", z_projectors.len());
    let const_signs_vec: Vec<(isize, isize, isize)> = [-1, 1]
        .into_iter()
        .flat_map(|val| {
            [-1, 1]
                .into_iter()
                .flat_map(|val| [-1, 1].into_iter().zip(repeat(val)))
                .zip(repeat(val))
        })
        .map(|((signx, signy), signz)| (signx, signy, signz))
        .collect::<Vec<(isize, isize, isize)>>();
    let mut best_candidate = V3 { x: 0, y: 0, z: 0 };
    //println!("{:?}", const_signs_vec);
    let mut best_score = isize::MAX;
    let mut progress = 0;
    println!();
    for x in x_projectors.iter(){
        println!("\rProgress: {}", progress);
        let _ = io::stdout().flush();
        for y in y_projectors.iter(){
            println!("Progress: {}", progress);
            for z in z_projectors.iter(){
                let next_candidates = const_signs_vec.iter().flat_map(|(signx, signy, signz)| {
                    [
                        V3 {
                            x: (x * signx),
                            y: (y * signy),
                            z: (z * signz),
                        },
                    ]
                        .into_iter()
                });
                for (candidate, score) in
                    next_candidates.map(|candidate| (candidate, get_idir_score(storms, candidate)))
                {
                    //println!("{:?} got score {}", candidate, score);
                    if score < best_score {
                        println!("\rnew best score: {}, in direction: {:?}", score, candidate);
                        let _ = io::stdout().flush();
                        println!("Progress: {}", progress);
                        (best_candidate, best_score) = (candidate, score)
                    }
                }
                if best_score == 0{
                    break;
                }
            }
            if best_score == 0{
                break;
            }
        }
        if best_score == 0{
            break;
        }
        progress += 1;
    }
    println!("best candidate: {:?}", best_candidate);
    //println!("{}", get_idir_score(storms, V3 { x: -3, y: 1, z: 2 }));
}
fn get_possible_z_projections(storm: &Storm<isize>, candidate_direction: &V3<isize>) -> Vec<isize>{
    let mut projected_z =
        storm.support.z.rem_euclid(candidate_direction.z);
    let mut possible_projections = vec![projected_z];
    loop {
        projected_z = projected_z + storm.direction.z;
        projected_z = projected_z.rem_euclid(candidate_direction.z);
        if possible_projections.contains(&projected_z)
        {
            break;
        } 
        possible_projections.push(projected_z);
    }
    possible_projections
}

fn get_projected_direction(storm: &Storm<isize>, candidate_direction: V3<isize>) -> V3<isize> {
    let mut projected_direction = storm.direction * (lcm(storm.direction.z, candidate_direction.z) / storm.direction.z);
    projected_direction = projected_direction - (candidate_direction * (projected_direction.z/candidate_direction.z));
    projected_direction
}

fn get_projected_support(storm: &Storm<isize>, candidate_direction: &V3<isize>, offset: isize) -> V3<isize>{
    let mut projected_support = storm.support;
    while projected_support.z.rem_euclid(candidate_direction.z) != offset {
        //println!("candidate direction: {:}, storm direction: {:}, support: {:?}, offset: {:}", candidate_direction.z, storm.direction.z, projected_support, projected_support.z.rem_euclid(candidate_direction.z));
        projected_support = projected_support + storm.direction;
    }
    projected_support = projected_support - *candidate_direction * (projected_support.z/candidate_direction.z);
    projected_support
}
pub fn get_idir_score(storms: &Vec<Storm<isize>>, candidate_direction: V3<isize>) -> isize {
    //println!("next candidate: {:?}", candidate_direction);
    if candidate_direction.z == 0.try_into().unwrap() {
        return isize::MAX
    }
    println!("mapping_storms");
    let possible_z_projections = storms
        .iter()
        .take(50)
        .map(|storm| {
            get_possible_z_projections(storm, &candidate_direction)
        }).collect::<Vec<_>>();
    //possible_z_projections.iter().for_each(|pz| println!("{:?}\n", pz));
    let offsets = (0..candidate_direction.z)
        .into_iter()
        .filter(|offset| {
            possible_z_projections
                .iter()
                .all(|possible_pz| possible_pz.contains(&offset))
        })
        .collect::<Vec<_>>();

    let offset;
    //println!("offsets: {:?}", offsets);
    if let Some(tmp_offset) = offsets.get(0) {
        offset = *tmp_offset;
    } else {
        return isize::MAX;
    }
    println!("candidate_offset: {offset}");

    let projected_storms = storms
        .into_iter()
        .map(|storm| {
            Storm {
                support: get_projected_support(&storm, &candidate_direction, offset),
                direction: get_projected_direction(storm, candidate_direction),
            }
        })
        .collect::<Vec<_>>();
    let collision_points = projected_storms
        .iter()
        .flat_map(|first_storm| {
            projected_storms
                .iter()
                .filter(move |&second_storm| first_storm != second_storm)
                .zip(repeat(first_storm))
        })
        .take(10)
        .filter_map(
            |(base_storm, other_storm)| {
                if base_storm.direction.y == 0 {
                    if other_storm.direction.y == 0
                        || (base_storm.direction.y - other_storm.direction.y)
                            % other_storm.direction.y
                            != 0
                    {
                        None
                    } else {
                        Some((
                            *base_storm,
                            Storm {
                                support: other_storm.support
                                    + other_storm.direction
                                        * (base_storm.support.y - other_storm.support.y)
                                        / other_storm.direction.y,
                                direction: other_storm.direction,
                            }
                        ))
                    }
                } else if other_storm.direction.y == 0 {
                    if base_storm.direction.y == 0
                        || (other_storm.direction.y - base_storm.direction.y)
                            % base_storm.direction.y
                            != 0
                    {
                        None
                    } else {
                        Some((
                            *other_storm,
                            Storm {
                                support: base_storm.support
                                    + base_storm.direction
                                        * (other_storm.support.y - base_storm.support.y)
                                        / base_storm.direction.y,
                                direction: base_storm.direction,
                            },
                        ))
                    }
                } else {
                    let mut new_base_support = base_storm.support;
                    let mut new_other_support = other_storm.support;
                    let new_base_direction = if base_storm.direction.y > 0 {
                        base_storm.direction
                    } else {
                        -base_storm.direction
                    };
                    let new_other_direction = if other_storm.direction.y > 0 {
                        other_storm.direction
                    } else {
                        -other_storm.direction
                    };
                    new_base_support = new_base_support
                        - new_base_direction
                            * ((new_base_support.y - new_other_support.y) / new_base_direction.y);
                    if (new_base_support.y - new_other_support.y) % gcd(new_base_direction.y, new_other_direction.y) != 0{
                        return None;
                    }
                    while new_base_support.y != new_other_support.y {
                        //println!("{:?}", new_base_direction);
                        //println!("{:?}\n", seen_diff);
                        if new_base_support.y < new_other_support.y {
                            new_base_support = new_base_support + new_base_direction;
                        } else {
                            new_other_support = new_other_support + new_other_direction;
                        }
                    }
                    let lcm_direction = lcm(new_base_direction.y, new_other_direction.y);
                    Some((
                        Storm {
                            support: new_base_support,
                            direction: new_base_direction * lcm_direction / new_base_direction.y,
                        },
                        Storm {
                            support: new_other_support,
                            direction: new_other_direction * lcm_direction / new_other_direction.y,
                        },
                    ))
                }
            },
        )
        .filter_map(|(storm1, storm2)| {
            if storm1.direction.x == storm2.direction.x {
                return None;
            }
            let crossing_at;
            if storm1.direction.y == 0.try_into().unwrap() {
                crossing_at = storm2.support;
            } else {
                let delta = (storm2.support.x - storm1.support.x)
                    / (storm1.direction.x - storm2.direction.x);
                crossing_at = storm1.support + storm1.direction * delta;
            }
            /*println!(
                "{}equal y at:\n{:?}\n{:?}\nvectors meet at:\n{:?}\n",
                _debug_record, storm1, storm2, crossing_at
            );*/
            Some(crossing_at)
        })
        .collect::<Vec<_>>();
    if collision_points.len() < 3 {
        return isize::MAX
    }
    let mean_point = collision_points.iter().fold(
        V3 {
            x: 0.try_into().unwrap(),
            y: 0.try_into().unwrap(),
            z: 0.try_into().unwrap(),
        },
        |sum, &next_in| sum + next_in,
    ) / collision_points.len() as isize;
    //println!("{:?}", mean_point);
    let ret = collision_points
        .iter()
        .map(|point| (*point - mean_point).manhatten())
        .sum::<isize>();
    if ret < 0{
        return isize::MAX
    }
    ret
}

pub fn bruteforce_direction(storms: &ec<Storm<f64>>) {
    let const_signs_vec: Vec<(i32, i32, i32)> = [-1, 1]
        .into_iter()
        .flat_map(|val| {
            [-1, 1]
                .into_iter()
                .flat_map(|val| [-1, 1].into_iter().zip(repeat(val)))
                .zip(repeat(val))
        })
        .map(|((signx, signy), signz)| (signx, signy, signz))
        .collect::<Vec<(i32, i32, i32)>>();
    let mut best_candidate = V3 {
        x: 0.0,
        y: 0.0,
        z: 0.0,
    };
    let mut best_score = f64::MAX;
    let mut x = 0;
    loop {
        for y in 0..x + 1 {
            for z in 0..y + 1 {
                let next_candidates = const_signs_vec.iter().map(|(signx, signy, signz)| V3 {
                    x: (x * signx).try_into().unwrap(),
                    y: (y * signy).try_into().unwrap(),
                    z: (z * signz).try_into().unwrap(),
                });
                (best_candidate, best_score) = next_candidates
                    .map(|candidate| (candidate, get_dir_score(storms, candidate)))
                    .fold(
                        (best_candidate, best_score),
                        |(best_candidate, best_score), (candidate, score)| {
                            if score < best_score {
                                println!(
                                    "new best score: {}, in direction: {:?}",
                                    score, candidate
                                );
                                (candidate, score)
                            } else {
                                (best_candidate, best_score)
                            }
                        },
                    );
            }
        }
        x += 1;
    }
}
fn part2_optimisation_attempt(storms: &Vec<Storm<f64>>) {
    let mut candidate = V3 {
        x: (0).try_into().unwrap(),
        y: (100).try_into().unwrap(),
        z: (-50).try_into().unwrap(),
    };
    let mut last_score = get_dir_score(storms, candidate);
    let mut next_tweak = 0;
    let mut last_successful_tweak = 0;
    let mut attempt_counter = 1;
    loop {
        let new_candidate = candidate
            + match next_tweak {
                0 => V3 {
                    x: 1.try_into().unwrap(),
                    y: 0.try_into().unwrap(),
                    z: 0.try_into().unwrap(),
                },
                1 => V3 {
                    x: (-1).try_into().unwrap(),
                    y: 0.try_into().unwrap(),
                    z: 0.try_into().unwrap(),
                },
                2 => V3 {
                    x: 0.try_into().unwrap(),
                    y: 1.try_into().unwrap(),
                    z: 0.try_into().unwrap(),
                },
                3 => V3 {
                    x: 0.try_into().unwrap(),
                    y: (-1).try_into().unwrap(),
                    z: 0.try_into().unwrap(),
                },
                4 => V3 {
                    x: 0.try_into().unwrap(),
                    y: 0.try_into().unwrap(),
                    z: 1.try_into().unwrap(),
                },
                _ => V3 {
                    x: 0.try_into().unwrap(),
                    y: 0.try_into().unwrap(),
                    z: (-1).try_into().unwrap(),
                },
            } * attempt_counter.try_into().unwrap();
        let new_score = get_dir_score(storms, new_candidate);
        if new_score < last_score {
            last_score = new_score;
            candidate = new_candidate;
            last_successful_tweak = next_tweak;
        } else {
            if attempt_counter < 1000 {
                attempt_counter += 1;
            } else {
                attempt_counter = 1;
                next_tweak = (next_tweak + 1) % 6;
                if next_tweak == last_successful_tweak {
                    panic!(
                        "failed to find direction. Last candidate was {:?} with score {}.",
                        candidate, last_score
                    );
                }
            }
        }
        if new_score == 0.try_into().unwrap() {
            println!("direction is: {:?}", new_candidate);
            break;
        }
    }
}

fn get_dir_score(storms: &Vec<Storm<f64>>, candidate_direction: V3<f64>) -> f64 {
    if candidate_direction.z == 0.try_into().unwrap() {
        return f64::MAX;
    }
    let projected_storms = storms
        .iter()
        .map(|storm| {
            (
                Storm {
                    support: storm.support
                        - candidate_direction * (storm.support.z / candidate_direction.z),
                    direction: storm.direction
                        - candidate_direction * (storm.direction.z / candidate_direction.z),
                },
                format!("{:?}", storm),
            )
        })
        .take(100)
        .collect::<Vec<_>>();
    let collision_points = projected_storms
        .iter()
        .flat_map(|first_storm| {
            projected_storms
                .iter()
                .filter(move |&second_storm| *first_storm != *second_storm)
                .zip(repeat(first_storm.clone()))
        })
        .take(20)
        .filter_map(
            |((base_storm, base_original), (other_storm, other_original))| {
                let record = format!(
                    "vectors:\n{}\n{}\nprojected vectors:\n{:?}\n{:?}\n",
                    base_original, other_original, base_storm, other_storm
                );
                if base_storm.direction.y != 0.try_into().unwrap() {
                    let s = (other_storm.support.y - base_storm.support.y) / base_storm.direction.y;
                    let t;
                    if other_storm.direction.y != 0.try_into().unwrap() {
                        t = base_storm.direction.y / other_storm.direction.y;
                    } else {
                        t = 1.try_into().unwrap();
                    }
                    Some((
                        Storm {
                            support: other_storm.support,
                            direction: other_storm.direction * t,
                        },
                        Storm {
                            support: base_storm.support + base_storm.direction * s,
                            direction: base_storm.direction,
                        },
                        record,
                    ))
                } else if other_storm.direction.y != 0.try_into().unwrap() {
                    let s =
                        (base_storm.support.y - other_storm.support.y) / other_storm.direction.y;
                    Some((
                        Storm {
                            support: base_storm.support,
                            direction: base_storm.direction,
                        },
                        Storm {
                            support: other_storm.support + other_storm.direction * s,
                            direction: other_storm.direction,
                        },
                        record,
                    ))
                } else {
                    None
                }
            },
        )
        .filter_map(|(storm1, storm2, _debug_record)| {
            if storm1.direction.x == storm2.direction.x {
                return None;
            }
            let crossing_at;
            if storm1.direction.y == 0.try_into().unwrap() {
                crossing_at = storm2.support;
            } else {
                let delta = (storm2.support.x - storm1.support.x)
                    / (storm1.direction.x - storm2.direction.x);
                crossing_at = storm1.support + storm1.direction * delta;
            }
            //println!("{}vectors meet at:\n{:?}\n", _debug_record, crossing_at);
            Some(crossing_at)
        })
        .collect::<Vec<_>>();
    let mean_point = collision_points.iter().fold(
        V3 {
            x: 0.try_into().unwrap(),
            y: 0.try_into().unwrap(),
            z: 0.try_into().unwrap(),
        },
        |mean, &next_in| mean + next_in / (collision_points.len() as f64).into(),
    );
    //println!("{:?}", mean_point);
    let ret = collision_points
        .iter()
        .map(|point| (*point - mean_point).manhatten())
        .sum::<f64>();
    ret
}
