class Calculator {
    constructor(initialValue) {
        this.value = initialValue;
    }

    add(num) {
        this.value += num;
        return this;
    }

    subtract(num) {
        this.value -= num;
        return this;
    }

    multiply(num) {
        this.value *= num;
        return this;
    }

    divide(num) {
        if (num === 0) throw new Error("Cannot divide by zero");
        this.value /= num;
        return this;
    }

    getValue() {
        return this.value;
    }
}

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
    const initialValue = 2.5;
    const calculator = new Calculator(initialValue);
    const result = performOperations(calculator);
    const roundedResult = roundToNearest(result.getValue(), 5);
    return roundedResult;
})();