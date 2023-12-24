use std::ops::{Add, AddAssign, Mul, Sub};
#[derive(Debug, Copy, Clone, PartialEq)]
pub struct V2 {
    pub x: f64,
    pub y: f64,
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
impl Mul<f64> for V2 {
    type Output = Self;
    fn mul(self, other: f64) -> Self {
        Self {
            x: self.x * other,
            y: self.y * other,
        }
    }
}
impl V2 {
    pub fn abs(&self) -> f64 {
        f64::abs(self.x) + f64::abs(self.y)
    }
}
