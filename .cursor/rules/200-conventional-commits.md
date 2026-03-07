---
author: Generated for Cursor IDE
version: 3.92.0
---
# Conventional Commits Specification

## Role

You are an AI assistant helping developers write clear, consistent, and meaningful Git commit messages following the Conventional Commits specification.

## Goal

When generating commit messages, always follow the Conventional Commits specification (https://www.conventionalcommits.org/). Use appropriate prefixes (feat, fix, docs, style, refactor, test, chore, etc.) and provide clear, specific descriptions that accurately describe the changes made. Reference .gitmessage file for the commit message format.

## Conventional Commits Format

The commit message should follow this structure:

```
<emoji> <type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```

### Commit Types

Use one of the following types based on the nature of the changes:

- **:sparkles: feat**: A new feature (introduces new functionality)
- **:bug: fix**: A bug fix (fixes an issue or bug)
- **:book: docs**: Documentation only changes (README, comments, API docs)
- **style**: Changes that do not affect the meaning of the code (formatting, whitespace, semicolons, etc.)
- **:recycle: refactor**: A code change that neither fixes a bug nor adds a feature (restructuring code)
- **:zap: perf**: A code change that improves performance
- **test**: Adding missing tests or correcting existing tests
- **:building_construction: build**: Changes that affect the build system or external dependencies (Maven, Gradle, npm, etc.)
- **:robot: ci**: Changes to CI configuration files and scripts (GitHub Actions, Jenkins, etc.)
- **:wrench: chore**: Other changes that don't modify src or test files (updating dependencies, config files, etc.)
- **:rewind: revert**: Reverts a previous commit

### Scope (Optional)

The scope is optional and should be the name of the package, module, or component affected:

- Examples: `:sparkles: feat(auth):`, `:bug: fix(api):`, `:recycle: refactor(core):`, `:book: docs(readme):`
- Use lowercase and be concise
- Use parentheses: `(scope)`

### Description

- Use imperative, present tense: "add" not "added" or "adds"
- Don't capitalize the first letter
- No period (.) at the end
- Be specific and clear about what changed
- Maximum 50 characters for the subject line

### Body (Optional)

- Provide additional context about the change
- Explain the "why" not the "what" (what is already in the subject)
- Wrap at 72 characters
- Use blank line to separate from subject

### Footer (Optional)

- Reference issues: `Closes #123`, `Fixes #456`
- Breaking changes: `BREAKING CHANGE: description`
- Co-authors: `Co-authored-by: Name <email>`

## Examples

### Good Examples

```
:sparkles: feat: add user authentication

Implement JWT-based authentication with refresh tokens.
Add login and logout endpoints.

Closes #123
```

```
:bug: fix(api): resolve null pointer exception in user service

The getUserById method was throwing NPE when user not found.
Added null check and proper error handling.

Fixes #456
```

```
:book: docs: update README with setup instructions

Added detailed setup instructions including environment
variables and database configuration.
```

```
:recycle: refactor(core): simplify dependency injection

Extracted common DI logic into utility class to reduce
code duplication across services.
```

```
:wrench: chore: update Maven dependencies to latest versions

Updated JUnit, Mockito, and Jackson to latest stable
versions for security patches.
```

```
:sparkles: feat(auth)!: change authentication API

BREAKING CHANGE: Authentication endpoints now require
API key in addition to JWT token.
```

### Bad Examples (Avoid)

```
❌ Updated code
❌ Fixed bug
❌ Changes
❌ WIP
❌ feat add authentication
❌ FEAT: Add authentication
❌ feat: Add authentication.
❌ feat: add authentication feature that allows users to login with their credentials and get a JWT token (too long)
```

## Instructions

1. **Analyze Changes**: Review the staged changes to understand what was modified
2. **Determine Type**: Identify the most appropriate commit type based on the changes
3. **Identify Scope**: If applicable, determine the scope (module/component affected)
4. **Write Description**: Create a clear, imperative description (max 50 chars)
5. **Add Body**: If needed, provide additional context in the body
6. **Add Footer**: Include issue references or breaking change notes if applicable

## Breaking Changes

When a commit introduces a breaking change:
- Add `!` after the type/scope: `feat!:` or `feat(api)!:`
- Include `BREAKING CHANGE:` in the footer with description

Example:
```
feat(api)!: change authentication endpoint

BREAKING CHANGE: The /auth/login endpoint now requires
API key header instead of query parameter.
```

## Constraints

- **MANDATORY**: Always use Conventional Commits format
- **MANDATORY**: Use one of the standard commit types
- **MANDATORY**: Keep subject line under 50 characters
- **MANDATORY**: Use imperative mood ("add" not "added")
- **MANDATORY**: Be specific and descriptive
- **NEVER**: Use generic messages like "update", "fix", "changes"
- **NEVER**: Capitalize the first letter of the subject
- **NEVER**: End subject with a period
- **NEVER**: Use past tense ("added", "fixed", "updated")

## Output Format

When generating commit messages:

1. **Subject Line**: `<type>[optional scope]: <description>`
2. **Body** (if needed): Additional context, blank line separated
3. **Footer** (if needed): Issue references or breaking changes

Always ensure the commit message:
- Follows Conventional Commits specification exactly
- Is clear and specific about what changed
- Uses appropriate type and scope
- Provides helpful context when needed

## Safeguards

- **VALIDATE**: Ensure the commit message matches Conventional Commits format before suggesting
- **REVIEW**: Check that the type accurately reflects the changes
- **VERIFY**: Confirm the description is clear and specific
- **CONFIRM**: Ensure breaking changes are properly marked
- **DOUBLE-CHECK**: Verify issue references are correct if included
