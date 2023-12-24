use std::ops::{Add, AddAssign, Mul, Sub};
#[derive(Eq, Hash, Debug, Copy, Clone, PartialEq)]
pub struct V2 {
    pub x: isize,
    pub y: isize,
}
impl Add for V2 {
    type Output = Self;
    fn add(self, other: Self) -> Self {
        Self {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}
impl AddAssign for V2 {
    fn add_assign(&mut self, other: Self) {
        *self = Self {
            x: self.x + other.x,
            y: self.y + other.y,
        };
    }
}
impl Sub for V2 {
    type Output = Self;
    fn sub(self, other: Self) -> Self {
        Self {
            x: self.x - other.x,
            y: self.y - other.y,
        }
    }
}
impl Mul<isize> for V2 {
    type Output = Self;
    fn mul(self, other: isize) -> Self {
        Self {
            x: self.x * other,
            y: self.y * other,
        }
    }
}
impl V2 {
    pub fn abs(&self) -> isize {
        isize::abs(self.x) + isize::abs(self.y)
    }
    pub fn is_on(&self, field_size: &Self) -> bool {
        if self.x < 0 || self.y < 0 || self.x >= field_size.x || self.y >= field_size.y {
            false
        } else {
            true
        }
    }
    pub fn get<T>(self, data: &Vec<Vec<T>>) -> Option<&T> {
        self.x
            .try_into()
            .map(|x: usize| {
                self.y
                    .try_into()
                    .map(|y: usize| data.get(y))
                    .ok()
                    .flatten()
                    .map(|line| line.get(x))
            })
            .ok()
            .flatten()
            .flatten()
    }
    pub fn get_mut<T>(self, data: &mut Vec<Vec<T>>) -> Option<&mut T> {
        self.x
            .try_into()
            .map(|x: usize| {
                self.y
                    .try_into()
                    .map(|y: usize| data.get_mut(y))
                    .ok()
                    .flatten()
                    .map(|line| line.get_mut(x))
            })
            .ok()
            .flatten()
            .flatten()
    }
}
pub const N: V2 = V2 { x: 0, y: -1 };
pub const E: V2 = V2 { x: 1, y: 0 };
pub const S: V2 = V2 { x: 0, y: 1 };
pub const W: V2 = V2 { x: -1, y: 0 };
