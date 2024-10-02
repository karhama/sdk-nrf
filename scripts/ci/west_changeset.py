#!/usr/bin/env python3
# SPDX-License-Identifier: Apache-2.0

# Copyright (c) 2024 Nordic Semiconductor ASA
import argparse
import yaml
import os
import requests
import re


def parse_args():
    """
    Parse command line arguments.

    Returns:
        argparse.Namespace: Parsed command line arguments.
    """
    parser = argparse.ArgumentParser(description="Get a list of changed files between two version of west manifest files.")
    parser.add_argument('old_west', help="Path to the old west.yml file.")
    parser.add_argument('new_west', help="Path to the new west.yml file.")
    return parser.parse_args()


def fetch_diff(repo_owner, repo_name, commit_range, token):
    """
    Fetch the diff from GitHub for a given range of commits.

    Args:
        repo_owner (str): Owner of the GitHub repository.
        repo_name (str): Name of the GitHub repository.
        commit_range (str): Commit range (e.g., "base_commit...head_commit").
        token (str): GitHub personal access token for authentication.

    Returns:
        str: The diff if the request is successful, None otherwise.
    """
    url = f"https://api.github.com/repos/{repo_owner}/{repo_name}/compare/{commit_range}"
    # Uses diff app to support large diff and avoid paging
    headers = {
        "Accept": "application/vnd.github.v3.diff",
        "Authorization": f"Bearer {token}",
        "X-GitHub-Api-Version": "2022-11-28"
    }

    response = requests.get(url, headers=headers)

    if response.status_code == 200:
        return response.text
    else:
        print(f"Error: {response.status_code}")
        return None


def extract_changed_files(diff_text):
    """
    Extract a list of changed files from the diff text.

    Args:
        diff_text (str): The diff text.

    Returns:
        list: A list of filenames that were changed.
    """
    # Use regular expression to find all lines starting with "diff --git a/"
    filenames = re.findall(r'^diff --git a/(.*?) b/.*$', diff_text, re.MULTILINE)
    return filenames

def main(old_west_yml, new_west_yml):
    """
    Main function to compare changed files between two west.yml revisions.

    Args:
        old_west_yml (str): Path to the old west.yml file.
        new_west_yml (str): Path to the new west.yml file.
    """
    with open(old_west_yml, 'r') as file:
        manifest = yaml.safe_load(file)
    projects = manifest.get('manifest', {}).get('projects', [])
    
    with open(new_west_yml, 'r') as file:
        new_manifest = yaml.safe_load(file)
    new_projects = new_manifest.get('manifest', {}).get('projects', [])

    changed_files = set()
    for idx, project in enumerate(projects):
        new_project = new_projects[idx]

        assert project['name'] == new_project['name'], "west.yml contains name changes"
        assert project.get('repo-path') == new_project.get('repo-path'), "west.yml contains repo-path changes"
        assert project.get('path') == new_project.get('path'), "west.yml contains path changes"
        
        repo_name = project.get('repo-path') if project.get('repo-path') else project['name']
        local_path = (project.get('path') if project.get('path') else repo_name) + '/'

        # Project not in nrfconnect github org (third party repo)
        # Will mark all project files as changed
        if project.get('remote'):
            if project['revision'] != new_project['revision']:
                changed_files.update(local_path + '*')
            continue

        # Project in nrfconnect github org - get changeset
        if project['revision'] != new_project['revision']:
            try:
                diff = fetch_diff('nrfconnect', repo_name, project['revision'] + '...' + new_project['revision'], os.environ['GITHUB_TOKEN'])
                filenames = extract_changed_files(diff)
                local_path = (project.get('path') if project.get('path') else repo_name) + '/'
                full_path_filenames = [local_path + file  for file in filenames]
            except Exception as e:
                print(f"Error determining git revisions for project {project['name']}")
                continue
            changed_files.update(full_path_filenames)

    changed_files_str = ' '.join(changed_files)
    print (changed_files_str)

if __name__ == '__main__':
    args = parse_args()
    main(args.old_west, args.new_west)

