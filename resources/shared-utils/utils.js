/**
 * Shared utility functions for MafiaMP resources.
 */

const utils = {
    /**
     * Randomly selects an element from an array.
     * @template T
     * @param {T[]} arr - The array containing elements.
     * @returns {T | undefined} Returns a random element, or undefined if array is empty.
     */
    getRandomInArray(arr) {
        if (!arr || arr.length === 0) return undefined;
        return arr[Math.floor(Math.random() * arr.length)];
    },

    /**
     * Returns an array containing the keys of an object.
     * @param {Object} obj - The object to get keys from.
     * @returns {string[]} Returns an array containing the keys.
     */
    getObjectKeys(obj) {
        return Object.keys(obj);
    }
};

// Export the utils object via Framework.Exports for cross-resource access
Framework.Exports.register("utils", utils);

// ES Module default export
export default utils;
