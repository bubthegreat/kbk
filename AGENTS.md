# AI Agent Instructions

This file contains instructions for AI coding agents working on this project.

---

## General Guideline

1. Use context7 where possible and check there before writing our own implementations, etc.
2. Use https://context7.com/hoffstadt/dearpygui specifically for the area editor UI

## Area Editor Guidelines

**SCOPE: These guidelines apply ONLY to the `area-editor/` portion of the codebase.**

The Area Editor is a standalone GUI-based area editor for KBK MUD, built with DearPyGui and Python using the UV package manager.

### 1. Feature Development

When adding new features to the area editor:

- **Update test.are**: Add new rooms to `tests/test.are` that demonstrate the feature
  - Follow minimalistic standards while maintaining a fantasy theme
  - Main theme: **Legend of the Red Dragon MUD**
  - Rooms should be simple but functional test cases
  
- **Document in FEATURES.md**: Add feature documentation following this format:
  ```
  ## Feature Name
  
  **Description**: Brief description of what the feature does
  
  **How to Test**:
  1. Step-by-step instructions to test the feature
  2. Expected behavior
  
  **Additional Notes**:
  - Any important implementation details
  - Known limitations
  - Related features
  
  ---
  ```

- **Add Tests**: Write pytest tests in `tests/` folder
  - Use standard pytest format
  - Tests must be runnable with `uv run pytest -v`
  - Test both happy path and edge cases

### 2. Exception Handling

When fixing exceptions:

- **Preserve Functionality**: Fix exceptions WITHOUT removing existing functionality unless explicitly directed by the user
- **Add Test Cases**: Create new rooms in `tests/test.are` with specific conditions that trigger the exception
- **Document the Fix**: Update FEATURES.md if the fix affects existing features

### 3. Dependency Management

**ALWAYS use UV package manager - NEVER edit lock files directly:**

- **Add dependencies**: `uv add <package>`
- **Add dev dependencies**: `uv add --dev <package>`
- **Update lock file**: UV handles this automatically
- **DO NOT**: Manually edit `uv.lock` or `pyproject.toml` dependencies

### 4. Code Quality

- **Type Hints**: All code must use Python type hints
- **Dataclasses**: Use dataclasses for all data models
- **Modular Design**: Prefer clean, modular code that would impress a senior engineer
- **Abstraction Limit**: Do not go more than 3 layers of inheritance/abstraction without asking
- **Testing**: Use pytest with pytest-xdist and pytest-cov for test coverage

### 5. Development Tools

Required tools (should already be installed):
- **ruff**: Linting and formatting
- **pytest**: Testing framework
- **pytest-xdist**: Parallel test execution
- **pytest-cov**: Test coverage reporting

### 6. Completeness Checklist

Before marking a feature as complete, ensure:

- [ ] Feature is implemented and working
- [ ] `tests/test.are` has rooms demonstrating the feature
- [ ] `FEATURES.md` is updated with feature documentation
- [ ] Tests are added to `tests/` folder
- [ ] All tests pass: `uv run pytest -v`
- [ ] No exceptions or errors in normal usage
- [ ] Code follows project conventions (type hints, dataclasses, etc.)

---

## KBK MUD Game Guidelines

**SCOPE: These guidelines apply to the actual MUD game portion of the codebase (NOT the area editor).**

### Coming Soon

Guidelines for the KBK MUD game will be added here as development progresses.

**Placeholder sections:**
- Game architecture and design patterns
- MUD-specific conventions (rooms, objects, mobiles, combat, etc.)
- Testing strategies for game mechanics
- Performance considerations
- Player experience guidelines

---

## General Notes

- **Ask Before Major Changes**: If a change affects multiple systems or requires significant refactoring, ask the user first
- **Follow Existing Patterns**: Look at existing code to understand conventions before adding new code
- **Document As You Go**: Keep FEATURES.md and this file up to date
- **Test Everything**: If it's not tested, it's not done

---

## Quick Reference

**Area Editor Commands:**
```bash
# Run tests
uv run pytest -v

# Run tests with coverage
uv run pytest --cov=area_editor tests/

# Run the area editor
uv run python -m area_editor

# Add a dependency
uv add <package>

# Add a dev dependency
uv add --dev <package>
```

**File Locations:**
- Area editor code: `src/area_editor/`
- Tests: `tests/`
- Test area file: `tests/test.are`
- Feature documentation: `FEATURES.md`
- Progress tracking: `PROGRESS.md`

