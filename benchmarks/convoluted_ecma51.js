function Calculator(initialValue) {
    this.value = initialValue;
}

Calculator.prototype.add = function(num) {
    this.value += num;
    return this;
};

Calculator.prototype.subtract = function(num) {
    this.value -= num;
    return this;
};

Calculator.prototype.multiply = function(num) {
    this.value *= num;
    return this;
};

Calculator.prototype.divide = function(num) {
    if (num === 0) throw new Error("Cannot divide by zero");
    this.value /= num;
    return this;
};

Calculator.prototype.getValue = function() {
    return this.value;
};

function performOperations(calculator) {
    return calculator
        .add(5)
        .multiply(4)
        .subtract(10)
        .divide(2)
        .add(7.5);
}

function roundToNearest(value, target) {
    return Math.round(value / target) * target;
}

(function() {
    var initialValue = 2.5;
    var calculator = new Calculator(initialValue);
    var result = performOperations(calculator);
    var roundedResult = roundToNearest(result.getValue(), 5);
    return roundedResult;
})();