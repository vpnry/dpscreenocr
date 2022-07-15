
#pragma once

#include <cstddef>


namespace dpso {


/**
 * Prettify text returned by Tesseract.
 *
 * The function:
 *   * Trims whitespace.
 *   * Splits fi and fl ligatures.
 *   * Removes paragraphs consisting of a single space, which are
 *       sometimes created when page segmentation is enabled.
 *
 * Returns new length of the text.
 */
std::size_t prettifyTesseractText(char* text);


}
